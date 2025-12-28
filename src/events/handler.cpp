#include "events/handler.hpp"

namespace temp2::events {

// =============================================================================
// DebouncedHandler
// =============================================================================

DebouncedHandler::DebouncedHandler(HandlerFunc handler, Duration delay)
    : handler_(std::move(handler))
    , delay_(delay)
    , last_call_(Clock::time_point::min()) {}

void DebouncedHandler::operator()(Event& event) {
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<Duration>(now - last_call_);

    if (elapsed >= delay_) {
        last_call_ = now;
        if (handler_) {
            handler_(event);
        }
    }
}

void DebouncedHandler::reset() {
    last_call_ = Clock::time_point::min();
}

// =============================================================================
// ThrottledHandler
// =============================================================================

ThrottledHandler::ThrottledHandler(HandlerFunc handler, Duration interval)
    : handler_(std::move(handler))
    , interval_(interval)
    , last_call_(Clock::time_point::min()) {}

void ThrottledHandler::operator()(Event& event) {
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<Duration>(now - last_call_);

    if (elapsed >= interval_) {
        last_call_ = now;
        if (handler_) {
            handler_(event);
        }
    }
}

void ThrottledHandler::reset() {
    last_call_ = Clock::time_point::min();
}

// =============================================================================
// OnceHandler
// =============================================================================

OnceHandler::OnceHandler(HandlerFunc handler)
    : handler_(std::move(handler)), fired_(false) {}

void OnceHandler::operator()(Event& event) {
    if (!fired_ && handler_) {
        fired_ = true;
        handler_(event);
    }
}

bool OnceHandler::has_fired() const {
    return fired_;
}

void OnceHandler::reset() {
    fired_ = false;
}

// =============================================================================
// ChainedHandler
// =============================================================================

ChainedHandler::ChainedHandler() = default;

ChainedHandler& ChainedHandler::then(HandlerFunc handler) {
    handlers_.push_back(std::move(handler));
    return *this;
}

void ChainedHandler::operator()(Event& event) {
    for (auto& handler : handlers_) {
        if (event.is_handled()) break;
        if (handler) {
            handler(event);
        }
    }
}

size_t ChainedHandler::handler_count() const {
    return handlers_.size();
}

}  // namespace temp2::events
