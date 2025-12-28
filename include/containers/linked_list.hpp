#ifndef TEMP2_CONTAINERS_LINKED_LIST_HPP
#define TEMP2_CONTAINERS_LINKED_LIST_HPP

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace temp2::containers {

/**
 * @brief Singly linked list node
 */
template <typename T>
struct SListNode {
    T data;
    SListNode* next;

    explicit SListNode(const T& value) : data(value), next(nullptr) {}
};

/**
 * @brief Singly linked list implementation
 */
template <typename T>
class SinglyLinkedList {
public:
    SinglyLinkedList();
    ~SinglyLinkedList();

    // Copy and move
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList(SinglyLinkedList&& other) noexcept;
    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;

    // Modifiers
    void push_front(const T& value);
    void push_back(const T& value);
    void insert_at(size_t index, const T& value);
    void pop_front();
    void pop_back();
    void remove_at(size_t index);
    void remove_value(const T& value);
    void clear();

    // Access
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& at(size_t index);
    const T& at(size_t index) const;
    std::optional<T> find(const T& value) const;

    // Properties
    size_t size() const;
    bool empty() const;
    bool contains(const T& value) const;
    size_t count(const T& value) const;

    // Operations
    void reverse();
    void sort();
    std::vector<T> to_vector() const;
    void for_each(const std::function<void(T&)>& fn);
    void for_each(const std::function<void(const T&)>& fn) const;

private:
    SListNode<T>* head_;
    SListNode<T>* tail_;
    size_t size_;

    void copy_from(const SinglyLinkedList& other);
};

/**
 * @brief Doubly linked list node
 */
template <typename T>
struct DListNode {
    T data;
    DListNode* prev;
    DListNode* next;

    explicit DListNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

/**
 * @brief Doubly linked list implementation
 */
template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    // Copy and move
    DoublyLinkedList(const DoublyLinkedList& other);
    DoublyLinkedList(DoublyLinkedList&& other) noexcept;
    DoublyLinkedList& operator=(const DoublyLinkedList& other);
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;

    // Modifiers
    void push_front(const T& value);
    void push_back(const T& value);
    void insert_at(size_t index, const T& value);
    void pop_front();
    void pop_back();
    void remove_at(size_t index);
    void clear();

    // Access
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& at(size_t index);
    const T& at(size_t index) const;

    // Properties
    size_t size() const;
    bool empty() const;

    // Operations
    void reverse();
    std::vector<T> to_vector() const;
    std::vector<T> to_vector_reverse() const;

    // Iteration
    void for_each_forward(const std::function<void(T&)>& fn);
    void for_each_backward(const std::function<void(T&)>& fn);

private:
    DListNode<T>* head_;
    DListNode<T>* tail_;
    size_t size_;

    void copy_from(const DoublyLinkedList& other);
};

}  // namespace temp2::containers

#endif  // TEMP2_CONTAINERS_LINKED_LIST_HPP
