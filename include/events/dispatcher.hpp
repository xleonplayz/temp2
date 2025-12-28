#ifndef TEMP2_EVENTS_DISPATCHER_HPP
#define TEMP2_EVENTS_DISPATCHER_HPP

#include "event.hpp"
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>

namespace temp2::events {

using EventHandler = std::function<void(Event&)>;
using HandlerId = size_t;

/**
 * @brief Event dispatcher with priority support
 */
class EventDispatcher {
public:
    EventDispatcher();
    ~EventDispatcher();

    // Subscribe to events
    HandlerId subscribe(const std::string& event_type, EventHandler handler, int priority = 0);
    void unsubscribe(HandlerId id);
    void unsubscribe(const std::string& event_type, HandlerId id);
    void unsubscribe_all(const std::string& event_type);
    void clear();

    // Dispatch events
    void dispatch(Event& event);
    void dispatch_immediate(Event& event);

    // Async dispatch
    void queue_event(std::unique_ptr<Event> event);
    void process_queue();
    size_t pending_events() const;

    // Query
    size_t handler_count(const std::string& event_type) const;
    bool has_handlers(const std::string& event_type) const;

private:
    struct HandlerEntry {
        HandlerId id;
        EventHandler handler;
        int priority;
    };

    std::map<std::string, std::vector<HandlerEntry>> handlers_;
    std::queue<std::unique_ptr<Event>> event_queue_;
    mutable std::mutex queue_mutex_;
    HandlerId next_id_;
};

/**
 * @brief Global event bus for application-wide events
 */
class EventBus {
public:
    static EventBus& instance();

    HandlerId subscribe(const std::string& event_type, EventHandler handler, int priority = 0);
    void unsubscribe(HandlerId id);
    void dispatch(Event& event);
    void queue_event(std::unique_ptr<Event> event);
    void process_queue();

    // Delete copy and move
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

private:
    EventBus();
    EventDispatcher dispatcher_;
};

/**
 * @brief RAII subscription guard
 */
class SubscriptionGuard {
public:
    SubscriptionGuard();
    SubscriptionGuard(EventDispatcher& dispatcher, HandlerId id);
    SubscriptionGuard(SubscriptionGuard&& other) noexcept;
    SubscriptionGuard& operator=(SubscriptionGuard&& other) noexcept;
    ~SubscriptionGuard();

    // Delete copy
    SubscriptionGuard(const SubscriptionGuard&) = delete;
    SubscriptionGuard& operator=(const SubscriptionGuard&) = delete;

    void release();
    HandlerId id() const;

private:
    EventDispatcher* dispatcher_;
    HandlerId id_;
};

}  // namespace temp2::events

#endif  // TEMP2_EVENTS_DISPATCHER_HPP
