#include "containers/linked_list.hpp"
#include <algorithm>
#include <stdexcept>

namespace temp2::containers {

// =============================================================================
// SinglyLinkedList
// =============================================================================

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    clear();
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList& other)
    : head_(nullptr), tail_(nullptr), size_(0) {
    copy_from(other);
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void SinglyLinkedList<T>::copy_from(const SinglyLinkedList& other) {
    SListNode<T>* current = other.head_;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T>
void SinglyLinkedList<T>::push_front(const T& value) {
    SListNode<T>* node = new SListNode<T>(value);
    node->next = head_;
    head_ = node;
    if (!tail_) {
        tail_ = node;
    }
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::push_back(const T& value) {
    SListNode<T>* node = new SListNode<T>(value);
    if (tail_) {
        tail_->next = node;
    } else {
        head_ = node;
    }
    tail_ = node;
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::insert_at(size_t index, const T& value) {
    if (index > size_) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        push_front(value);
        return;
    }

    if (index == size_) {
        push_back(value);
        return;
    }

    SListNode<T>* prev = head_;
    for (size_t i = 0; i < index - 1; ++i) {
        prev = prev->next;
    }

    SListNode<T>* node = new SListNode<T>(value);
    node->next = prev->next;
    prev->next = node;
    ++size_;
}

template <typename T>
void SinglyLinkedList<T>::pop_front() {
    if (empty()) {
        throw std::runtime_error("Pop from empty list");
    }

    SListNode<T>* node = head_;
    head_ = head_->next;
    if (!head_) {
        tail_ = nullptr;
    }
    delete node;
    --size_;
}

template <typename T>
void SinglyLinkedList<T>::pop_back() {
    if (empty()) {
        throw std::runtime_error("Pop from empty list");
    }

    if (size_ == 1) {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
        return;
    }

    SListNode<T>* prev = head_;
    while (prev->next != tail_) {
        prev = prev->next;
    }

    delete tail_;
    tail_ = prev;
    tail_->next = nullptr;
    --size_;
}

template <typename T>
void SinglyLinkedList<T>::remove_at(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        pop_front();
        return;
    }

    SListNode<T>* prev = head_;
    for (size_t i = 0; i < index - 1; ++i) {
        prev = prev->next;
    }

    SListNode<T>* node = prev->next;
    prev->next = node->next;

    if (node == tail_) {
        tail_ = prev;
    }

    delete node;
    --size_;
}

template <typename T>
void SinglyLinkedList<T>::remove_value(const T& value) {
    SListNode<T>* prev = nullptr;
    SListNode<T>* current = head_;

    while (current) {
        if (current->data == value) {
            if (prev) {
                prev->next = current->next;
            } else {
                head_ = current->next;
            }

            if (current == tail_) {
                tail_ = prev;
            }

            delete current;
            --size_;
            return;
        }
        prev = current;
        current = current->next;
    }
}

template <typename T>
void SinglyLinkedList<T>::clear() {
    while (head_) {
        SListNode<T>* node = head_;
        head_ = head_->next;
        delete node;
    }
    tail_ = nullptr;
    size_ = 0;
}

template <typename T>
T& SinglyLinkedList<T>::front() {
    if (empty()) throw std::runtime_error("Empty list");
    return head_->data;
}

template <typename T>
const T& SinglyLinkedList<T>::front() const {
    if (empty()) throw std::runtime_error("Empty list");
    return head_->data;
}

template <typename T>
T& SinglyLinkedList<T>::back() {
    if (empty()) throw std::runtime_error("Empty list");
    return tail_->data;
}

template <typename T>
const T& SinglyLinkedList<T>::back() const {
    if (empty()) throw std::runtime_error("Empty list");
    return tail_->data;
}

template <typename T>
T& SinglyLinkedList<T>::at(size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");

    SListNode<T>* current = head_;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
const T& SinglyLinkedList<T>::at(size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");

    SListNode<T>* current = head_;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
std::optional<T> SinglyLinkedList<T>::find(const T& value) const {
    SListNode<T>* current = head_;
    while (current) {
        if (current->data == value) {
            return current->data;
        }
        current = current->next;
    }
    return std::nullopt;
}

template <typename T>
size_t SinglyLinkedList<T>::size() const {
    return size_;
}

template <typename T>
bool SinglyLinkedList<T>::empty() const {
    return size_ == 0;
}

template <typename T>
bool SinglyLinkedList<T>::contains(const T& value) const {
    return find(value).has_value();
}

template <typename T>
size_t SinglyLinkedList<T>::count(const T& value) const {
    size_t cnt = 0;
    SListNode<T>* current = head_;
    while (current) {
        if (current->data == value) {
            ++cnt;
        }
        current = current->next;
    }
    return cnt;
}

template <typename T>
void SinglyLinkedList<T>::reverse() {
    SListNode<T>* prev = nullptr;
    SListNode<T>* current = head_;
    tail_ = head_;

    while (current) {
        SListNode<T>* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    head_ = prev;
}

template <typename T>
void SinglyLinkedList<T>::sort() {
    if (size_ <= 1) return;

    std::vector<T> vec = to_vector();
    std::sort(vec.begin(), vec.end());

    clear();
    for (const T& val : vec) {
        push_back(val);
    }
}

template <typename T>
std::vector<T> SinglyLinkedList<T>::to_vector() const {
    std::vector<T> result;
    result.reserve(size_);

    SListNode<T>* current = head_;
    while (current) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

template <typename T>
void SinglyLinkedList<T>::for_each(const std::function<void(T&)>& fn) {
    SListNode<T>* current = head_;
    while (current) {
        fn(current->data);
        current = current->next;
    }
}

template <typename T>
void SinglyLinkedList<T>::for_each(const std::function<void(const T&)>& fn) const {
    SListNode<T>* current = head_;
    while (current) {
        fn(current->data);
        current = current->next;
    }
}

// =============================================================================
// DoublyLinkedList
// =============================================================================

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
    : head_(nullptr), tail_(nullptr), size_(0) {
    copy_from(other);
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void DoublyLinkedList<T>::copy_from(const DoublyLinkedList& other) {
    DListNode<T>* current = other.head_;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T>
void DoublyLinkedList<T>::push_front(const T& value) {
    DListNode<T>* node = new DListNode<T>(value);
    node->next = head_;

    if (head_) {
        head_->prev = node;
    } else {
        tail_ = node;
    }

    head_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::push_back(const T& value) {
    DListNode<T>* node = new DListNode<T>(value);
    node->prev = tail_;

    if (tail_) {
        tail_->next = node;
    } else {
        head_ = node;
    }

    tail_ = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::insert_at(size_t index, const T& value) {
    if (index > size_) {
        throw std::out_of_range("Index out of range");
    }

    if (index == 0) {
        push_front(value);
        return;
    }

    if (index == size_) {
        push_back(value);
        return;
    }

    DListNode<T>* current;
    if (index < size_ / 2) {
        current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail_;
        for (size_t i = size_ - 1; i > index; --i) {
            current = current->prev;
        }
    }

    DListNode<T>* node = new DListNode<T>(value);
    node->prev = current->prev;
    node->next = current;
    current->prev->next = node;
    current->prev = node;
    ++size_;
}

template <typename T>
void DoublyLinkedList<T>::pop_front() {
    if (empty()) throw std::runtime_error("Pop from empty list");

    DListNode<T>* node = head_;
    head_ = head_->next;

    if (head_) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }

    delete node;
    --size_;
}

template <typename T>
void DoublyLinkedList<T>::pop_back() {
    if (empty()) throw std::runtime_error("Pop from empty list");

    DListNode<T>* node = tail_;
    tail_ = tail_->prev;

    if (tail_) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }

    delete node;
    --size_;
}

template <typename T>
void DoublyLinkedList<T>::remove_at(size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");

    if (index == 0) {
        pop_front();
        return;
    }

    if (index == size_ - 1) {
        pop_back();
        return;
    }

    DListNode<T>* current;
    if (index < size_ / 2) {
        current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail_;
        for (size_t i = size_ - 1; i > index; --i) {
            current = current->prev;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    --size_;
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while (head_) {
        DListNode<T>* node = head_;
        head_ = head_->next;
        delete node;
    }
    tail_ = nullptr;
    size_ = 0;
}

template <typename T>
T& DoublyLinkedList<T>::front() {
    if (empty()) throw std::runtime_error("Empty list");
    return head_->data;
}

template <typename T>
const T& DoublyLinkedList<T>::front() const {
    if (empty()) throw std::runtime_error("Empty list");
    return head_->data;
}

template <typename T>
T& DoublyLinkedList<T>::back() {
    if (empty()) throw std::runtime_error("Empty list");
    return tail_->data;
}

template <typename T>
const T& DoublyLinkedList<T>::back() const {
    if (empty()) throw std::runtime_error("Empty list");
    return tail_->data;
}

template <typename T>
T& DoublyLinkedList<T>::at(size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");

    DListNode<T>* current;
    if (index < size_ / 2) {
        current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail_;
        for (size_t i = size_ - 1; i > index; --i) {
            current = current->prev;
        }
    }
    return current->data;
}

template <typename T>
const T& DoublyLinkedList<T>::at(size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");

    DListNode<T>* current;
    if (index < size_ / 2) {
        current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail_;
        for (size_t i = size_ - 1; i > index; --i) {
            current = current->prev;
        }
    }
    return current->data;
}

template <typename T>
size_t DoublyLinkedList<T>::size() const {
    return size_;
}

template <typename T>
bool DoublyLinkedList<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void DoublyLinkedList<T>::reverse() {
    DListNode<T>* current = head_;
    std::swap(head_, tail_);

    while (current) {
        std::swap(current->prev, current->next);
        current = current->prev;
    }
}

template <typename T>
std::vector<T> DoublyLinkedList<T>::to_vector() const {
    std::vector<T> result;
    result.reserve(size_);

    DListNode<T>* current = head_;
    while (current) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

template <typename T>
std::vector<T> DoublyLinkedList<T>::to_vector_reverse() const {
    std::vector<T> result;
    result.reserve(size_);

    DListNode<T>* current = tail_;
    while (current) {
        result.push_back(current->data);
        current = current->prev;
    }

    return result;
}

template <typename T>
void DoublyLinkedList<T>::for_each_forward(const std::function<void(T&)>& fn) {
    DListNode<T>* current = head_;
    while (current) {
        fn(current->data);
        current = current->next;
    }
}

template <typename T>
void DoublyLinkedList<T>::for_each_backward(const std::function<void(T&)>& fn) {
    DListNode<T>* current = tail_;
    while (current) {
        fn(current->data);
        current = current->prev;
    }
}

// Explicit template instantiations for common types
template class SinglyLinkedList<int>;
template class SinglyLinkedList<double>;
template class SinglyLinkedList<std::string>;
template class DoublyLinkedList<int>;
template class DoublyLinkedList<double>;
template class DoublyLinkedList<std::string>;

}  // namespace temp2::containers
