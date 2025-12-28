#include "events/dispatcher.hpp"
#include <algorithm>

namespace temp2::events {

// =============================================================================
// EventDispatcher
// =============================================================================

EventDispatcher::EventDispatcher() : next_id_(1) {}

EventDispatcher::~EventDispatcher() {
    clear();
}

HandlerId EventDispatcher::subscribe(const std::string& event_type, EventHandler handler, int priority) {
    HandlerId id = next_id_++;

    HandlerEntry entry{id, std::move(handler), priority};
    auto& handler_list = handlers_[event_type];

    // Insert sorted by priority (higher priority first)
    auto it = std::lower_bound(handler_list.begin(), handler_list.end(), entry,
        [](const HandlerEntry& a, const HandlerEntry& b) {
            return a.priority > b.priority;
        });
    handler_list.insert(it, std::move(entry));

    return id;
}

void EventDispatcher::unsubscribe(HandlerId id) {
    for (auto& [type, handler_list] : handlers_) {
        auto it = std::remove_if(handler_list.begin(), handler_list.end(),
            [id](const HandlerEntry& entry) { return entry.id == id; });
        handler_list.erase(it, handler_list.end());
    }
}

void EventDispatcher::unsubscribe(const std::string& event_type, HandlerId id) {
    auto it = handlers_.find(event_type);
    if (it != handlers_.end()) {
        auto& handler_list = it->second;
        auto remove_it = std::remove_if(handler_list.begin(), handler_list.end(),
            [id](const HandlerEntry& entry) { return entry.id == id; });
        handler_list.erase(remove_it, handler_list.end());
    }
}

void EventDispatcher::unsubscribe_all(const std::string& event_type) {
    handlers_.erase(event_type);
}

void EventDispatcher::clear() {
    handlers_.clear();
    std::lock_guard<std::mutex> lock(queue_mutex_);
    while (!event_queue_.empty()) {
        event_queue_.pop();
    }
}

void EventDispatcher::dispatch(Event& event) {
    auto it = handlers_.find(event.type());
    if (it == handlers_.end()) return;

    for (auto& entry : it->second) {
        if (event.is_handled()) break;
        entry.handler(event);
    }
}

void EventDispatcher::dispatch_immediate(Event& event) {
    dispatch(event);
}

void EventDispatcher::queue_event(std::unique_ptr<Event> event) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    event_queue_.push(std::move(event));
}

void EventDispatcher::process_queue() {
    std::queue<std::unique_ptr<Event>> local_queue;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        std::swap(local_queue, event_queue_);
    }

    while (!local_queue.empty()) {
        auto event = std::move(local_queue.front());
        local_queue.pop();
        dispatch(*event);
    }
}

size_t EventDispatcher::pending_events() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return event_queue_.size();
}

size_t EventDispatcher::handler_count(const std::string& event_type) const {
    auto it = handlers_.find(event_type);
    if (it != handlers_.end()) {
        return it->second.size();
    }
    return 0;
}

bool EventDispatcher::has_handlers(const std::string& event_type) const {
    return handler_count(event_type) > 0;
}

// =============================================================================
// EventBus
// =============================================================================

EventBus::EventBus() = default;

EventBus& EventBus::instance() {
    static EventBus instance;
    return instance;
}

HandlerId EventBus::subscribe(const std::string& event_type, EventHandler handler, int priority) {
    return dispatcher_.subscribe(event_type, std::move(handler), priority);
}

void EventBus::unsubscribe(HandlerId id) {
    dispatcher_.unsubscribe(id);
}

void EventBus::dispatch(Event& event) {
    dispatcher_.dispatch(event);
}

void EventBus::queue_event(std::unique_ptr<Event> event) {
    dispatcher_.queue_event(std::move(event));
}

void EventBus::process_queue() {
    dispatcher_.process_queue();
}

// =============================================================================
// SubscriptionGuard
// =============================================================================

SubscriptionGuard::SubscriptionGuard() : dispatcher_(nullptr), id_(0) {}

SubscriptionGuard::SubscriptionGuard(EventDispatcher& dispatcher, HandlerId id)
    : dispatcher_(&dispatcher), id_(id) {}

SubscriptionGuard::SubscriptionGuard(SubscriptionGuard&& other) noexcept
    : dispatcher_(other.dispatcher_), id_(other.id_) {
    other.dispatcher_ = nullptr;
    other.id_ = 0;
}

SubscriptionGuard& SubscriptionGuard::operator=(SubscriptionGuard&& other) noexcept {
    if (this != &other) {
        if (dispatcher_ && id_ != 0) {
            dispatcher_->unsubscribe(id_);
        }
        dispatcher_ = other.dispatcher_;
        id_ = other.id_;
        other.dispatcher_ = nullptr;
        other.id_ = 0;
    }
    return *this;
}

SubscriptionGuard::~SubscriptionGuard() {
    if (dispatcher_ && id_ != 0) {
        dispatcher_->unsubscribe(id_);
    }
}

void SubscriptionGuard::release() {
    dispatcher_ = nullptr;
    id_ = 0;
}

HandlerId SubscriptionGuard::id() const {
    return id_;
}

}  // namespace temp2::events
