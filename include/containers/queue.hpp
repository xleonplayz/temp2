#ifndef TEMP2_CONTAINERS_QUEUE_HPP
#define TEMP2_CONTAINERS_QUEUE_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace temp2::containers {

/**
 * @brief Circular buffer-based queue
 */
template <typename T>
class CircularQueue {
public:
    explicit CircularQueue(size_t capacity);
    ~CircularQueue();

    CircularQueue(const CircularQueue& other);
    CircularQueue(CircularQueue&& other) noexcept;
    CircularQueue& operator=(const CircularQueue& other);
    CircularQueue& operator=(CircularQueue&& other) noexcept;

    bool enqueue(const T& value);
    std::optional<T> dequeue();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    bool full() const;
    void clear();

private:
    T* data_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;
};

/**
 * @brief Dynamic array-based queue
 */
template <typename T>
class ArrayQueue {
public:
    ArrayQueue();

    void enqueue(const T& value);
    void enqueue(T&& value);
    T dequeue();
    std::optional<T> try_dequeue();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_t size() const;
    bool empty() const;
    void clear();

    std::vector<T> to_vector() const;

private:
    std::vector<T> data_;
    size_t front_index_;
};

/**
 * @brief Double-ended queue (deque)
 */
template <typename T>
class Deque {
public:
    Deque();

    void push_front(const T& value);
    void push_back(const T& value);
    T pop_front();
    T pop_back();
    std::optional<T> try_pop_front();
    std::optional<T> try_pop_back();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& at(size_t index);
    const T& at(size_t index) const;

    size_t size() const;
    bool empty() const;
    void clear();

    std::vector<T> to_vector() const;

private:
    std::vector<T> data_;
    size_t front_index_;
    size_t back_index_;
    size_t size_;

    void grow_front();
    void grow_back();
};

/**
 * @brief Priority queue (max-heap by default)
 */
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue();
    explicit PriorityQueue(const Compare& comp);

    void push(const T& value);
    T pop();
    std::optional<T> try_pop();
    const T& top() const;

    size_t size() const;
    bool empty() const;
    void clear();

    std::vector<T> to_sorted_vector();

private:
    std::vector<T> heap_;
    Compare compare_;

    void sift_up(size_t index);
    void sift_down(size_t index);
    size_t parent(size_t index) const;
    size_t left_child(size_t index) const;
    size_t right_child(size_t index) const;
};

}  // namespace temp2::containers

#endif  // TEMP2_CONTAINERS_QUEUE_HPP
