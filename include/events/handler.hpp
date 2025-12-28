#ifndef TEMP2_EVENTS_HANDLER_HPP
#define TEMP2_EVENTS_HANDLER_HPP

#include "event.hpp"
#include "dispatcher.hpp"
#include <functional>
#include <type_traits>

namespace temp2::events {

/**
 * @brief Type-safe event handler
 */
template <typename EventType>
class TypedEventHandler {
public:
    using HandlerFunc = std::function<void(EventType&)>;

    TypedEventHandler() = default;
    explicit TypedEventHandler(HandlerFunc handler) : handler_(std::move(handler)) {}

    void operator()(Event& event) {
        if (handler_) {
            EventType& typed_event = static_cast<EventType&>(event);
            handler_(typed_event);
        }
    }

    bool is_valid() const { return static_cast<bool>(handler_); }

private:
    HandlerFunc handler_;
};

/**
 * @brief Event filter
 */
template <typename EventType>
class EventFilter {
public:
    using FilterFunc = std::function<bool(const EventType&)>;

    EventFilter() = default;
    explicit EventFilter(FilterFunc filter) : filter_(std::move(filter)) {}

    bool accepts(const Event& event) const {
        if (!filter_) return true;
        const EventType& typed_event = static_cast<const EventType&>(event);
        return filter_(typed_event);
    }

private:
    FilterFunc filter_;
};

/**
 * @brief Event handler with automatic type dispatch
 */
class EventHandlerBase {
public:
    virtual ~EventHandlerBase() = default;
    virtual void handle(Event& event) = 0;
    virtual bool can_handle(const std::string& type) const = 0;
};

template <typename EventType>
class AutoEventHandler : public EventHandlerBase {
public:
    using HandlerFunc = std::function<void(EventType&)>;

    explicit AutoEventHandler(const std::string& event_type, HandlerFunc handler)
        : event_type_(event_type), handler_(std::move(handler)) {}

    void handle(Event& event) override {
        if (handler_ && event.type() == event_type_) {
            handler_(static_cast<EventType&>(event));
        }
    }

    bool can_handle(const std::string& type) const override {
        return type == event_type_;
    }

private:
    std::string event_type_;
    HandlerFunc handler_;
};

/**
 * @brief Debounced event handler
 */
class DebouncedHandler {
public:
    using HandlerFunc = std::function<void(Event&)>;
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::milliseconds;

    DebouncedHandler(HandlerFunc handler, Duration delay);

    void operator()(Event& event);
    void reset();

private:
    HandlerFunc handler_;
    Duration delay_;
    Clock::time_point last_call_;
};

/**
 * @brief Throttled event handler
 */
class ThrottledHandler {
public:
    using HandlerFunc = std::function<void(Event&)>;
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::milliseconds;

    ThrottledHandler(HandlerFunc handler, Duration interval);

    void operator()(Event& event);
    void reset();

private:
    HandlerFunc handler_;
    Duration interval_;
    Clock::time_point last_call_;
};

/**
 * @brief Once handler - only fires once
 */
class OnceHandler {
public:
    using HandlerFunc = std::function<void(Event&)>;

    explicit OnceHandler(HandlerFunc handler);

    void operator()(Event& event);
    bool has_fired() const;
    void reset();

private:
    HandlerFunc handler_;
    bool fired_;
};

/**
 * @brief Chained handler - calls multiple handlers in sequence
 */
class ChainedHandler {
public:
    using HandlerFunc = std::function<void(Event&)>;

    ChainedHandler();

    ChainedHandler& then(HandlerFunc handler);
    void operator()(Event& event);
    size_t handler_count() const;

private:
    std::vector<HandlerFunc> handlers_;
};

}  // namespace temp2::events

#endif  // TEMP2_EVENTS_HANDLER_HPP
