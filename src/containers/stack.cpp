#include "containers/stack.hpp"

namespace temp2::containers {

// =============================================================================
// ArrayStack
// =============================================================================

template <typename T>
ArrayStack<T>::ArrayStack() {}

template <typename T>
ArrayStack<T>::ArrayStack(size_t initial_capacity) {
    data_.reserve(initial_capacity);
}

template <typename T>
void ArrayStack<T>::push(const T& value) {
    data_.push_back(value);
}

template <typename T>
void ArrayStack<T>::push(T&& value) {
    data_.push_back(std::move(value));
}

template <typename T>
T ArrayStack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Pop from empty stack");
    }
    T value = std::move(data_.back());
    data_.pop_back();
    return value;
}

template <typename T>
std::optional<T> ArrayStack<T>::try_pop() {
    if (empty()) {
        return std::nullopt;
    }
    T value = std::move(data_.back());
    data_.pop_back();
    return value;
}

template <typename T>
T& ArrayStack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
const T& ArrayStack<T>::top() const {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
std::optional<T> ArrayStack<T>::try_top() const {
    if (empty()) {
        return std::nullopt;
    }
    return data_.back();
}

template <typename T>
size_t ArrayStack<T>::size() const {
    return data_.size();
}

template <typename T>
bool ArrayStack<T>::empty() const {
    return data_.empty();
}

template <typename T>
size_t ArrayStack<T>::capacity() const {
    return data_.capacity();
}

template <typename T>
void ArrayStack<T>::clear() {
    data_.clear();
}

template <typename T>
void ArrayStack<T>::reserve(size_t new_capacity) {
    data_.reserve(new_capacity);
}

template <typename T>
std::vector<T> ArrayStack<T>::to_vector() const {
    return data_;
}

// =============================================================================
// LinkedStack
// =============================================================================

template <typename T>
LinkedStack<T>::LinkedStack() : top_(nullptr), size_(0) {}

template <typename T>
LinkedStack<T>::~LinkedStack() {
    clear();
}

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack& other) : top_(nullptr), size_(0) {
    copy_from(other);
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack&& other) noexcept
    : top_(other.top_), size_(other.size_) {
    other.top_ = nullptr;
    other.size_ = 0;
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack&& other) noexcept {
    if (this != &other) {
        clear();
        top_ = other.top_;
        size_ = other.size_;
        other.top_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void LinkedStack<T>::copy_from(const LinkedStack& other) {
    if (!other.top_) return;

    // Create a temporary vector to reverse order
    std::vector<T> values;
    Node* current = other.top_;
    while (current) {
        values.push_back(current->data);
        current = current->next;
    }

    // Push in reverse to maintain same order
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        push(*it);
    }
}

template <typename T>
void LinkedStack<T>::push(const T& value) {
    Node* node = new Node(value);
    node->next = top_;
    top_ = node;
    ++size_;
}

template <typename T>
T LinkedStack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Pop from empty stack");
    }
    Node* node = top_;
    T value = std::move(node->data);
    top_ = top_->next;
    delete node;
    --size_;
    return value;
}

template <typename T>
std::optional<T> LinkedStack<T>::try_pop() {
    if (empty()) {
        return std::nullopt;
    }
    return pop();
}

template <typename T>
T& LinkedStack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return top_->data;
}

template <typename T>
const T& LinkedStack<T>::top() const {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return top_->data;
}

template <typename T>
size_t LinkedStack<T>::size() const {
    return size_;
}

template <typename T>
bool LinkedStack<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void LinkedStack<T>::clear() {
    while (top_) {
        Node* node = top_;
        top_ = top_->next;
        delete node;
    }
    size_ = 0;
}

// =============================================================================
// MinStack
// =============================================================================

template <typename T>
MinStack<T>::MinStack() {}

template <typename T>
void MinStack<T>::push(const T& value) {
    data_.push_back(value);
    if (min_stack_.empty() || value <= min_stack_.back()) {
        min_stack_.push_back(value);
    }
}

template <typename T>
T MinStack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Pop from empty stack");
    }
    T value = data_.back();
    data_.pop_back();

    if (value == min_stack_.back()) {
        min_stack_.pop_back();
    }

    return value;
}

template <typename T>
T& MinStack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
const T& MinStack<T>::top() const {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
T MinStack<T>::min() const {
    if (empty()) {
        throw std::runtime_error("Min of empty stack");
    }
    return min_stack_.back();
}

template <typename T>
size_t MinStack<T>::size() const {
    return data_.size();
}

template <typename T>
bool MinStack<T>::empty() const {
    return data_.empty();
}

template <typename T>
void MinStack<T>::clear() {
    data_.clear();
    min_stack_.clear();
}

// =============================================================================
// MaxStack
// =============================================================================

template <typename T>
MaxStack<T>::MaxStack() {}

template <typename T>
void MaxStack<T>::push(const T& value) {
    data_.push_back(value);
    if (max_stack_.empty() || value >= max_stack_.back()) {
        max_stack_.push_back(value);
    }
}

template <typename T>
T MaxStack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Pop from empty stack");
    }
    T value = data_.back();
    data_.pop_back();

    if (value == max_stack_.back()) {
        max_stack_.pop_back();
    }

    return value;
}

template <typename T>
T& MaxStack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
const T& MaxStack<T>::top() const {
    if (empty()) {
        throw std::runtime_error("Top of empty stack");
    }
    return data_.back();
}

template <typename T>
T MaxStack<T>::max() const {
    if (empty()) {
        throw std::runtime_error("Max of empty stack");
    }
    return max_stack_.back();
}

template <typename T>
size_t MaxStack<T>::size() const {
    return data_.size();
}

template <typename T>
bool MaxStack<T>::empty() const {
    return data_.empty();
}

template <typename T>
void MaxStack<T>::clear() {
    data_.clear();
    max_stack_.clear();
}

// Explicit template instantiations
template class ArrayStack<int>;
template class ArrayStack<double>;
template class ArrayStack<std::string>;
template class LinkedStack<int>;
template class LinkedStack<double>;
template class LinkedStack<std::string>;
template class MinStack<int>;
template class MinStack<double>;
template class MaxStack<int>;
template class MaxStack<double>;

}  // namespace temp2::containers
