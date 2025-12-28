#include "events/event.hpp"

namespace temp2::events {

// =============================================================================
// Event
// =============================================================================

Event::Event() : type_("unknown"), timestamp_(std::chrono::steady_clock::now()), handled_(false) {}

Event::Event(const std::string& type)
    : type_(type), timestamp_(std::chrono::steady_clock::now()), handled_(false) {}

const std::string& Event::type() const { return type_; }
Event::Timestamp Event::timestamp() const { return timestamp_; }
bool Event::is_handled() const { return handled_; }
void Event::set_handled(bool handled) { handled_ = handled; }

void Event::set_data(const std::string& key, const std::any& value) {
    data_[key] = value;
}

std::any Event::get_data(const std::string& key) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return it->second;
    }
    return {};
}

bool Event::has_data(const std::string& key) const {
    return data_.find(key) != data_.end();
}

// =============================================================================
// MouseEvent
// =============================================================================

MouseEvent::MouseEvent(Action action, double x, double y, Button button)
    : Event("mouse"), action_(action), button_(button), x_(x), y_(y), scroll_delta_(0) {}

MouseEvent::Action MouseEvent::action() const { return action_; }
MouseEvent::Button MouseEvent::button() const { return button_; }
double MouseEvent::x() const { return x_; }
double MouseEvent::y() const { return y_; }
double MouseEvent::scroll_delta() const { return scroll_delta_; }
void MouseEvent::set_scroll_delta(double delta) { scroll_delta_ = delta; }

// =============================================================================
// KeyboardEvent
// =============================================================================

KeyboardEvent::KeyboardEvent(Action action, int key_code, int modifiers)
    : Event("keyboard"), action_(action), key_code_(key_code), modifiers_(modifiers) {}

KeyboardEvent::Action KeyboardEvent::action() const { return action_; }
int KeyboardEvent::key_code() const { return key_code_; }
int KeyboardEvent::modifiers() const { return modifiers_; }
bool KeyboardEvent::is_shift() const { return modifiers_ & MOD_SHIFT; }
bool KeyboardEvent::is_ctrl() const { return modifiers_ & MOD_CTRL; }
bool KeyboardEvent::is_alt() const { return modifiers_ & MOD_ALT; }

// =============================================================================
// WindowEvent
// =============================================================================

WindowEvent::WindowEvent(Action action)
    : Event("window"), action_(action), width_(0), height_(0), x_(0), y_(0) {}

WindowEvent::WindowEvent(Action action, int width, int height)
    : Event("window"), action_(action), width_(width), height_(height), x_(0), y_(0) {}

WindowEvent::WindowEvent(Action action, int x, int y, bool)
    : Event("window"), action_(action), width_(0), height_(0), x_(x), y_(y) {}

WindowEvent::Action WindowEvent::action() const { return action_; }
int WindowEvent::width() const { return width_; }
int WindowEvent::height() const { return height_; }
int WindowEvent::x() const { return x_; }
int WindowEvent::y() const { return y_; }

// =============================================================================
// CustomEvent
// =============================================================================

CustomEvent::CustomEvent(const std::string& name)
    : Event("custom"), name_(name) {}

const std::string& CustomEvent::name() const { return name_; }

}  // namespace temp2::events
