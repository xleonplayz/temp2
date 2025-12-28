#ifndef TEMP2_CONTAINERS_STACK_HPP
#define TEMP2_CONTAINERS_STACK_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace temp2::containers {

/**
 * @brief Array-based stack implementation
 */
template <typename T>
class ArrayStack {
public:
    ArrayStack();
    explicit ArrayStack(size_t initial_capacity);

    void push(const T& value);
    void push(T&& value);
    T pop();
    std::optional<T> try_pop();
    T& top();
    const T& top() const;
    std::optional<T> try_top() const;

    size_t size() const;
    bool empty() const;
    size_t capacity() const;
    void clear();
    void reserve(size_t new_capacity);

    std::vector<T> to_vector() const;

private:
    std::vector<T> data_;
};

/**
 * @brief Linked list-based stack implementation
 */
template <typename T>
class LinkedStack {
public:
    LinkedStack();
    ~LinkedStack();

    LinkedStack(const LinkedStack& other);
    LinkedStack(LinkedStack&& other) noexcept;
    LinkedStack& operator=(const LinkedStack& other);
    LinkedStack& operator=(LinkedStack&& other) noexcept;

    void push(const T& value);
    T pop();
    std::optional<T> try_pop();
    T& top();
    const T& top() const;

    size_t size() const;
    bool empty() const;
    void clear();

private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* top_;
    size_t size_;

    void copy_from(const LinkedStack& other);
};

/**
 * @brief Min-stack that tracks minimum element
 */
template <typename T>
class MinStack {
public:
    MinStack();

    void push(const T& value);
    T pop();
    T& top();
    const T& top() const;
    T min() const;

    size_t size() const;
    bool empty() const;
    void clear();

private:
    std::vector<T> data_;
    std::vector<T> min_stack_;
};

/**
 * @brief Max-stack that tracks maximum element
 */
template <typename T>
class MaxStack {
public:
    MaxStack();

    void push(const T& value);
    T pop();
    T& top();
    const T& top() const;
    T max() const;

    size_t size() const;
    bool empty() const;
    void clear();

private:
    std::vector<T> data_;
    std::vector<T> max_stack_;
};

}  // namespace temp2::containers

#endif  // TEMP2_CONTAINERS_STACK_HPP
