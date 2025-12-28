#ifndef TEMP2_EVENTS_EVENT_HPP
#define TEMP2_EVENTS_EVENT_HPP

#include <string>
#include <any>
#include <chrono>
#include <map>

namespace temp2::events {

/**
 * @brief Base event class
 */
class Event {
public:
    using Timestamp = std::chrono::steady_clock::time_point;

    Event();
    explicit Event(const std::string& type);
    virtual ~Event() = default;

    const std::string& type() const;
    Timestamp timestamp() const;
    bool is_handled() const;
    void set_handled(bool handled = true);

    // Data storage
    void set_data(const std::string& key, const std::any& value);
    std::any get_data(const std::string& key) const;
    bool has_data(const std::string& key) const;

    template <typename T>
    T get_data_as(const std::string& key) const {
        return std::any_cast<T>(data_.at(key));
    }

protected:
    std::string type_;
    Timestamp timestamp_;
    bool handled_;
    std::map<std::string, std::any> data_;
};

/**
 * @brief Mouse event
 */
class MouseEvent : public Event {
public:
    enum class Button { Left, Right, Middle, None };
    enum class Action { Press, Release, Move, Scroll };

    MouseEvent(Action action, double x, double y, Button button = Button::None);

    Action action() const;
    Button button() const;
    double x() const;
    double y() const;
    double scroll_delta() const;

    void set_scroll_delta(double delta);

private:
    Action action_;
    Button button_;
    double x_;
    double y_;
    double scroll_delta_;
};

/**
 * @brief Keyboard event
 */
class KeyboardEvent : public Event {
public:
    enum class Action { Press, Release, Repeat };

    KeyboardEvent(Action action, int key_code, int modifiers = 0);

    Action action() const;
    int key_code() const;
    int modifiers() const;

    bool is_shift() const;
    bool is_ctrl() const;
    bool is_alt() const;

    static constexpr int MOD_SHIFT = 1;
    static constexpr int MOD_CTRL = 2;
    static constexpr int MOD_ALT = 4;

private:
    Action action_;
    int key_code_;
    int modifiers_;
};

/**
 * @brief Window event
 */
class WindowEvent : public Event {
public:
    enum class Action { Resize, Move, Close, Focus, Blur, Minimize, Maximize };

    WindowEvent(Action action);
    WindowEvent(Action action, int width, int height);
    WindowEvent(Action action, int x, int y, bool is_position);

    Action action() const;
    int width() const;
    int height() const;
    int x() const;
    int y() const;

private:
    Action action_;
    int width_;
    int height_;
    int x_;
    int y_;
};

/**
 * @brief Custom event with arbitrary data
 */
class CustomEvent : public Event {
public:
    explicit CustomEvent(const std::string& name);

    const std::string& name() const;

private:
    std::string name_;
};

}  // namespace temp2::events

#endif  // TEMP2_EVENTS_EVENT_HPP
