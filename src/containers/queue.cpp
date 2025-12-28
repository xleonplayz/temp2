#include "containers/queue.hpp"
#include <algorithm>

namespace temp2::containers {

// =============================================================================
// CircularQueue
// =============================================================================

template <typename T>
CircularQueue<T>::CircularQueue(size_t capacity)
    : capacity_(capacity), head_(0), tail_(0), size_(0) {
    data_ = new T[capacity_];
}

template <typename T>
CircularQueue<T>::~CircularQueue() {
    delete[] data_;
}

template <typename T>
CircularQueue<T>::CircularQueue(const CircularQueue& other)
    : capacity_(other.capacity_), head_(other.head_), tail_(other.tail_), size_(other.size_) {
    data_ = new T[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <typename T>
CircularQueue<T>::CircularQueue(CircularQueue&& other) noexcept
    : data_(other.data_), capacity_(other.capacity_),
      head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
}

template <typename T>
CircularQueue<T>& CircularQueue<T>::operator=(const CircularQueue& other) {
    if (this != &other) {
        delete[] data_;
        capacity_ = other.capacity_;
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        data_ = new T[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    return *this;
}

template <typename T>
CircularQueue<T>& CircularQueue<T>::operator=(CircularQueue&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        capacity_ = other.capacity_;
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
bool CircularQueue<T>::enqueue(const T& value) {
    if (full()) {
        return false;
    }
    data_[tail_] = value;
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
    return true;
}

template <typename T>
std::optional<T> CircularQueue<T>::dequeue() {
    if (empty()) {
        return std::nullopt;
    }
    T value = std::move(data_[head_]);
    head_ = (head_ + 1) % capacity_;
    --size_;
    return value;
}

template <typename T>
T& CircularQueue<T>::front() {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[head_];
}

template <typename T>
const T& CircularQueue<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[head_];
}

template <typename T>
T& CircularQueue<T>::back() {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[(tail_ + capacity_ - 1) % capacity_];
}

template <typename T>
const T& CircularQueue<T>::back() const {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[(tail_ + capacity_ - 1) % capacity_];
}

template <typename T>
size_t CircularQueue<T>::size() const {
    return size_;
}

template <typename T>
size_t CircularQueue<T>::capacity() const {
    return capacity_;
}

template <typename T>
bool CircularQueue<T>::empty() const {
    return size_ == 0;
}

template <typename T>
bool CircularQueue<T>::full() const {
    return size_ == capacity_;
}

template <typename T>
void CircularQueue<T>::clear() {
    head_ = 0;
    tail_ = 0;
    size_ = 0;
}

// =============================================================================
// ArrayQueue
// =============================================================================

template <typename T>
ArrayQueue<T>::ArrayQueue() : front_index_(0) {}

template <typename T>
void ArrayQueue<T>::enqueue(const T& value) {
    data_.push_back(value);
}

template <typename T>
void ArrayQueue<T>::enqueue(T&& value) {
    data_.push_back(std::move(value));
}

template <typename T>
T ArrayQueue<T>::dequeue() {
    if (empty()) {
        throw std::runtime_error("Dequeue from empty queue");
    }
    T value = std::move(data_[front_index_]);
    ++front_index_;

    // Compact if too much wasted space
    if (front_index_ > data_.size() / 2) {
        data_.erase(data_.begin(), data_.begin() + front_index_);
        front_index_ = 0;
    }

    return value;
}

template <typename T>
std::optional<T> ArrayQueue<T>::try_dequeue() {
    if (empty()) {
        return std::nullopt;
    }
    return dequeue();
}

template <typename T>
T& ArrayQueue<T>::front() {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[front_index_];
}

template <typename T>
const T& ArrayQueue<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_[front_index_];
}

template <typename T>
T& ArrayQueue<T>::back() {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_.back();
}

template <typename T>
const T& ArrayQueue<T>::back() const {
    if (empty()) {
        throw std::runtime_error("Empty queue");
    }
    return data_.back();
}

template <typename T>
size_t ArrayQueue<T>::size() const {
    return data_.size() - front_index_;
}

template <typename T>
bool ArrayQueue<T>::empty() const {
    return front_index_ >= data_.size();
}

template <typename T>
void ArrayQueue<T>::clear() {
    data_.clear();
    front_index_ = 0;
}

template <typename T>
std::vector<T> ArrayQueue<T>::to_vector() const {
    return std::vector<T>(data_.begin() + front_index_, data_.end());
}

// =============================================================================
// Deque
// =============================================================================

template <typename T>
Deque<T>::Deque() : front_index_(0), back_index_(0), size_(0) {}

template <typename T>
void Deque<T>::grow_front() {
    size_t new_size = data_.empty() ? 8 : data_.size() * 2;
    std::vector<T> new_data(new_size);

    size_t new_front = new_size / 4;
    for (size_t i = 0; i < size_; ++i) {
        new_data[new_front + i] = std::move(data_[front_index_ + i]);
    }

    data_ = std::move(new_data);
    front_index_ = new_front;
    back_index_ = front_index_ + size_;
}

template <typename T>
void Deque<T>::grow_back() {
    size_t new_size = data_.empty() ? 8 : data_.size() * 2;
    std::vector<T> new_data(new_size);

    size_t new_front = new_size / 4;
    for (size_t i = 0; i < size_; ++i) {
        new_data[new_front + i] = std::move(data_[front_index_ + i]);
    }

    data_ = std::move(new_data);
    front_index_ = new_front;
    back_index_ = front_index_ + size_;
}

template <typename T>
void Deque<T>::push_front(const T& value) {
    if (front_index_ == 0) {
        grow_front();
    }
    --front_index_;
    data_[front_index_] = value;
    ++size_;
}

template <typename T>
void Deque<T>::push_back(const T& value) {
    if (back_index_ >= data_.size()) {
        grow_back();
    }
    data_[back_index_] = value;
    ++back_index_;
    ++size_;
}

template <typename T>
T Deque<T>::pop_front() {
    if (empty()) {
        throw std::runtime_error("Pop from empty deque");
    }
    T value = std::move(data_[front_index_]);
    ++front_index_;
    --size_;
    return value;
}

template <typename T>
T Deque<T>::pop_back() {
    if (empty()) {
        throw std::runtime_error("Pop from empty deque");
    }
    --back_index_;
    --size_;
    return std::move(data_[back_index_]);
}

template <typename T>
std::optional<T> Deque<T>::try_pop_front() {
    if (empty()) {
        return std::nullopt;
    }
    return pop_front();
}

template <typename T>
std::optional<T> Deque<T>::try_pop_back() {
    if (empty()) {
        return std::nullopt;
    }
    return pop_back();
}

template <typename T>
T& Deque<T>::front() {
    if (empty()) {
        throw std::runtime_error("Empty deque");
    }
    return data_[front_index_];
}

template <typename T>
const T& Deque<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Empty deque");
    }
    return data_[front_index_];
}

template <typename T>
T& Deque<T>::back() {
    if (empty()) {
        throw std::runtime_error("Empty deque");
    }
    return data_[back_index_ - 1];
}

template <typename T>
const T& Deque<T>::back() const {
    if (empty()) {
        throw std::runtime_error("Empty deque");
    }
    return data_[back_index_ - 1];
}

template <typename T>
T& Deque<T>::at(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[front_index_ + index];
}

template <typename T>
const T& Deque<T>::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[front_index_ + index];
}

template <typename T>
size_t Deque<T>::size() const {
    return size_;
}

template <typename T>
bool Deque<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void Deque<T>::clear() {
    data_.clear();
    front_index_ = 0;
    back_index_ = 0;
    size_ = 0;
}

template <typename T>
std::vector<T> Deque<T>::to_vector() const {
    return std::vector<T>(data_.begin() + front_index_, data_.begin() + back_index_);
}

// =============================================================================
// PriorityQueue
// =============================================================================

template <typename T, typename Compare>
PriorityQueue<T, Compare>::PriorityQueue() : compare_(Compare()) {}

template <typename T, typename Compare>
PriorityQueue<T, Compare>::PriorityQueue(const Compare& comp) : compare_(comp) {}

template <typename T, typename Compare>
size_t PriorityQueue<T, Compare>::parent(size_t index) const {
    return (index - 1) / 2;
}

template <typename T, typename Compare>
size_t PriorityQueue<T, Compare>::left_child(size_t index) const {
    return 2 * index + 1;
}

template <typename T, typename Compare>
size_t PriorityQueue<T, Compare>::right_child(size_t index) const {
    return 2 * index + 2;
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::sift_up(size_t index) {
    while (index > 0) {
        size_t p = parent(index);
        if (compare_(heap_[p], heap_[index])) {
            std::swap(heap_[p], heap_[index]);
            index = p;
        } else {
            break;
        }
    }
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::sift_down(size_t index) {
    size_t n = heap_.size();
    while (true) {
        size_t largest = index;
        size_t left = left_child(index);
        size_t right = right_child(index);

        if (left < n && compare_(heap_[largest], heap_[left])) {
            largest = left;
        }
        if (right < n && compare_(heap_[largest], heap_[right])) {
            largest = right;
        }

        if (largest != index) {
            std::swap(heap_[index], heap_[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::push(const T& value) {
    heap_.push_back(value);
    sift_up(heap_.size() - 1);
}

template <typename T, typename Compare>
T PriorityQueue<T, Compare>::pop() {
    if (empty()) {
        throw std::runtime_error("Pop from empty priority queue");
    }

    T result = heap_[0];
    heap_[0] = heap_.back();
    heap_.pop_back();

    if (!empty()) {
        sift_down(0);
    }

    return result;
}

template <typename T, typename Compare>
std::optional<T> PriorityQueue<T, Compare>::try_pop() {
    if (empty()) {
        return std::nullopt;
    }
    return pop();
}

template <typename T, typename Compare>
const T& PriorityQueue<T, Compare>::top() const {
    if (empty()) {
        throw std::runtime_error("Top of empty priority queue");
    }
    return heap_[0];
}

template <typename T, typename Compare>
size_t PriorityQueue<T, Compare>::size() const {
    return heap_.size();
}

template <typename T, typename Compare>
bool PriorityQueue<T, Compare>::empty() const {
    return heap_.empty();
}

template <typename T, typename Compare>
void PriorityQueue<T, Compare>::clear() {
    heap_.clear();
}

template <typename T, typename Compare>
std::vector<T> PriorityQueue<T, Compare>::to_sorted_vector() {
    std::vector<T> result;
    result.reserve(heap_.size());

    while (!empty()) {
        result.push_back(pop());
    }

    return result;
}

// Explicit template instantiations
template class CircularQueue<int>;
template class CircularQueue<double>;
template class CircularQueue<std::string>;
template class ArrayQueue<int>;
template class ArrayQueue<double>;
template class ArrayQueue<std::string>;
template class Deque<int>;
template class Deque<double>;
template class Deque<std::string>;
template class PriorityQueue<int>;
template class PriorityQueue<double>;
template class PriorityQueue<int, std::greater<int>>;
template class PriorityQueue<double, std::greater<double>>;

}  // namespace temp2::containers
