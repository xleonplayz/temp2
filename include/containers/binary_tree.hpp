#ifndef TEMP2_CONTAINERS_BINARY_TREE_HPP
#define TEMP2_CONTAINERS_BINARY_TREE_HPP

#include <functional>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace temp2::containers {

/**
 * @brief Binary tree node
 */
template <typename T>
struct TreeNode {
    T data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    explicit TreeNode(const T& value)
        : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
};

/**
 * @brief Binary Search Tree implementation
 */
template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
public:
    BinarySearchTree();
    explicit BinarySearchTree(const Compare& comp);
    ~BinarySearchTree();

    BinarySearchTree(const BinarySearchTree& other);
    BinarySearchTree(BinarySearchTree&& other) noexcept;
    BinarySearchTree& operator=(const BinarySearchTree& other);
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    // Modifiers
    void insert(const T& value);
    bool remove(const T& value);
    void clear();

    // Search
    bool contains(const T& value) const;
    std::optional<T> find(const T& value) const;
    std::optional<T> find_min() const;
    std::optional<T> find_max() const;
    std::optional<T> floor(const T& value) const;
    std::optional<T> ceiling(const T& value) const;

    // Properties
    size_t size() const;
    bool empty() const;
    int height() const;
    bool is_balanced() const;
    bool is_valid_bst() const;

    // Traversals
    std::vector<T> inorder() const;
    std::vector<T> preorder() const;
    std::vector<T> postorder() const;
    std::vector<T> level_order() const;

    void inorder_visit(const std::function<void(const T&)>& visitor) const;
    void preorder_visit(const std::function<void(const T&)>& visitor) const;
    void postorder_visit(const std::function<void(const T&)>& visitor) const;

    // Nth element
    std::optional<T> kth_smallest(size_t k) const;
    std::optional<T> kth_largest(size_t k) const;

private:
    TreeNode<T>* root_;
    size_t size_;
    Compare compare_;

    TreeNode<T>* copy_tree(TreeNode<T>* node);
    void delete_tree(TreeNode<T>* node);
    TreeNode<T>* insert_node(TreeNode<T>* node, const T& value);
    TreeNode<T>* remove_node(TreeNode<T>* node, const T& value, bool& removed);
    TreeNode<T>* find_min_node(TreeNode<T>* node) const;
    TreeNode<T>* find_max_node(TreeNode<T>* node) const;
    int calculate_height(TreeNode<T>* node) const;
    bool check_balanced(TreeNode<T>* node) const;
    bool validate_bst(TreeNode<T>* node, const T* min, const T* max) const;
    void inorder_traverse(TreeNode<T>* node, std::vector<T>& result) const;
    void preorder_traverse(TreeNode<T>* node, std::vector<T>& result) const;
    void postorder_traverse(TreeNode<T>* node, std::vector<T>& result) const;
};

/**
 * @brief AVL Tree (self-balancing BST)
 */
template <typename T, typename Compare = std::less<T>>
class AVLTree {
public:
    AVLTree();
    explicit AVLTree(const Compare& comp);
    ~AVLTree();

    void insert(const T& value);
    bool remove(const T& value);
    bool contains(const T& value) const;
    void clear();

    size_t size() const;
    bool empty() const;
    int height() const;

    std::vector<T> inorder() const;
    std::optional<T> find_min() const;
    std::optional<T> find_max() const;

private:
    struct AVLNode {
        T data;
        AVLNode* left;
        AVLNode* right;
        int height;

        explicit AVLNode(const T& value)
            : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    AVLNode* root_;
    size_t size_;
    Compare compare_;

    int get_height(AVLNode* node) const;
    int get_balance(AVLNode* node) const;
    void update_height(AVLNode* node);

    AVLNode* rotate_left(AVLNode* node);
    AVLNode* rotate_right(AVLNode* node);
    AVLNode* rebalance(AVLNode* node);

    AVLNode* insert_node(AVLNode* node, const T& value);
    AVLNode* remove_node(AVLNode* node, const T& value, bool& removed);
    AVLNode* find_min_node(AVLNode* node) const;

    void delete_tree(AVLNode* node);
    void inorder_traverse(AVLNode* node, std::vector<T>& result) const;
};

/**
 * @brief Trie (prefix tree) for strings
 */
class Trie {
public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool starts_with(const std::string& prefix) const;
    bool remove(const std::string& word);

    size_t word_count() const;
    size_t node_count() const;
    bool empty() const;
    void clear();

    std::vector<std::string> get_all_words() const;
    std::vector<std::string> get_words_with_prefix(const std::string& prefix) const;
    std::vector<std::string> autocomplete(const std::string& prefix, size_t max_results = 10) const;

private:
    struct TrieNode {
        TrieNode* children[26];
        bool is_end;
        int count;

        TrieNode() : is_end(false), count(0) {
            for (int i = 0; i < 26; ++i) {
                children[i] = nullptr;
            }
        }
    };

    TrieNode* root_;
    size_t word_count_;
    size_t node_count_;

    void delete_tree(TrieNode* node);
    void collect_words(TrieNode* node, const std::string& prefix,
                       std::vector<std::string>& words) const;
    TrieNode* find_node(const std::string& prefix) const;
    bool remove_helper(TrieNode* node, const std::string& word, size_t depth);
};

}  // namespace temp2::containers

#endif  // TEMP2_CONTAINERS_BINARY_TREE_HPP
