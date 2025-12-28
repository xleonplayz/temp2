#include "containers/binary_tree.hpp"
#include <algorithm>
#include <limits>

namespace temp2::containers {

// =============================================================================
// BinarySearchTree
// =============================================================================

template <typename T, typename Compare>
BinarySearchTree<T, Compare>::BinarySearchTree() : root_(nullptr), size_(0), compare_(Compare()) {}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>::BinarySearchTree(const Compare& comp)
    : root_(nullptr), size_(0), compare_(comp) {}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>::~BinarySearchTree() {
    delete_tree(root_);
}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>::BinarySearchTree(const BinarySearchTree& other)
    : root_(nullptr), size_(0), compare_(other.compare_) {
    root_ = copy_tree(other.root_);
    size_ = other.size_;
}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>::BinarySearchTree(BinarySearchTree&& other) noexcept
    : root_(other.root_), size_(other.size_), compare_(std::move(other.compare_)) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>& BinarySearchTree<T, Compare>::operator=(const BinarySearchTree& other) {
    if (this != &other) {
        delete_tree(root_);
        compare_ = other.compare_;
        root_ = copy_tree(other.root_);
        size_ = other.size_;
    }
    return *this;
}

template <typename T, typename Compare>
BinarySearchTree<T, Compare>& BinarySearchTree<T, Compare>::operator=(BinarySearchTree&& other) noexcept {
    if (this != &other) {
        delete_tree(root_);
        root_ = other.root_;
        size_ = other.size_;
        compare_ = std::move(other.compare_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T, typename Compare>
TreeNode<T>* BinarySearchTree<T, Compare>::copy_tree(TreeNode<T>* node) {
    if (!node) return nullptr;

    TreeNode<T>* new_node = new TreeNode<T>(node->data);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);

    if (new_node->left) new_node->left->parent = new_node;
    if (new_node->right) new_node->right->parent = new_node;

    return new_node;
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::delete_tree(TreeNode<T>* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::insert(const T& value) {
    root_ = insert_node(root_, value);
}

template <typename T, typename Compare>
TreeNode<T>* BinarySearchTree<T, Compare>::insert_node(TreeNode<T>* node, const T& value) {
    if (!node) {
        ++size_;
        return new TreeNode<T>(value);
    }

    if (compare_(value, node->data)) {
        node->left = insert_node(node->left, value);
        node->left->parent = node;
    } else if (compare_(node->data, value)) {
        node->right = insert_node(node->right, value);
        node->right->parent = node;
    }
    // If equal, don't insert (no duplicates)

    return node;
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::remove(const T& value) {
    bool removed = false;
    root_ = remove_node(root_, value, removed);
    return removed;
}

template <typename T, typename Compare>
TreeNode<T>* BinarySearchTree<T, Compare>::remove_node(TreeNode<T>* node, const T& value, bool& removed) {
    if (!node) return nullptr;

    if (compare_(value, node->data)) {
        node->left = remove_node(node->left, value, removed);
    } else if (compare_(node->data, value)) {
        node->right = remove_node(node->right, value, removed);
    } else {
        removed = true;
        --size_;

        // Node with no children
        if (!node->left && !node->right) {
            delete node;
            return nullptr;
        }

        // Node with one child
        if (!node->left) {
            TreeNode<T>* right = node->right;
            right->parent = node->parent;
            delete node;
            return right;
        }
        if (!node->right) {
            TreeNode<T>* left = node->left;
            left->parent = node->parent;
            delete node;
            return left;
        }

        // Node with two children
        TreeNode<T>* successor = find_min_node(node->right);
        node->data = successor->data;
        ++size_;  // Compensate since recursive remove will decrement
        node->right = remove_node(node->right, successor->data, removed);
    }

    return node;
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::clear() {
    delete_tree(root_);
    root_ = nullptr;
    size_ = 0;
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::contains(const T& value) const {
    TreeNode<T>* node = root_;
    while (node) {
        if (compare_(value, node->data)) {
            node = node->left;
        } else if (compare_(node->data, value)) {
            node = node->right;
        } else {
            return true;
        }
    }
    return false;
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::find(const T& value) const {
    TreeNode<T>* node = root_;
    while (node) {
        if (compare_(value, node->data)) {
            node = node->left;
        } else if (compare_(node->data, value)) {
            node = node->right;
        } else {
            return node->data;
        }
    }
    return std::nullopt;
}

template <typename T, typename Compare>
TreeNode<T>* BinarySearchTree<T, Compare>::find_min_node(TreeNode<T>* node) const {
    if (!node) return nullptr;
    while (node->left) {
        node = node->left;
    }
    return node;
}

template <typename T, typename Compare>
TreeNode<T>* BinarySearchTree<T, Compare>::find_max_node(TreeNode<T>* node) const {
    if (!node) return nullptr;
    while (node->right) {
        node = node->right;
    }
    return node;
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::find_min() const {
    TreeNode<T>* node = find_min_node(root_);
    return node ? std::optional<T>(node->data) : std::nullopt;
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::find_max() const {
    TreeNode<T>* node = find_max_node(root_);
    return node ? std::optional<T>(node->data) : std::nullopt;
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::floor(const T& value) const {
    TreeNode<T>* node = root_;
    TreeNode<T>* result = nullptr;

    while (node) {
        if (!compare_(value, node->data)) {
            result = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }

    return result ? std::optional<T>(result->data) : std::nullopt;
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::ceiling(const T& value) const {
    TreeNode<T>* node = root_;
    TreeNode<T>* result = nullptr;

    while (node) {
        if (!compare_(node->data, value)) {
            result = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return result ? std::optional<T>(result->data) : std::nullopt;
}

template <typename T, typename Compare>
size_t BinarySearchTree<T, Compare>::size() const {
    return size_;
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::empty() const {
    return size_ == 0;
}

template <typename T, typename Compare>
int BinarySearchTree<T, Compare>::calculate_height(TreeNode<T>* node) const {
    if (!node) return -1;
    return 1 + std::max(calculate_height(node->left), calculate_height(node->right));
}

template <typename T, typename Compare>
int BinarySearchTree<T, Compare>::height() const {
    return calculate_height(root_);
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::check_balanced(TreeNode<T>* node) const {
    if (!node) return true;

    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);

    if (std::abs(left_height - right_height) > 1) {
        return false;
    }

    return check_balanced(node->left) && check_balanced(node->right);
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::is_balanced() const {
    return check_balanced(root_);
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::validate_bst(TreeNode<T>* node, const T* min, const T* max) const {
    if (!node) return true;

    if (min && !compare_(*min, node->data)) return false;
    if (max && !compare_(node->data, *max)) return false;

    return validate_bst(node->left, min, &node->data) &&
           validate_bst(node->right, &node->data, max);
}

template <typename T, typename Compare>
bool BinarySearchTree<T, Compare>::is_valid_bst() const {
    return validate_bst(root_, nullptr, nullptr);
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::inorder_traverse(TreeNode<T>* node, std::vector<T>& result) const {
    if (!node) return;
    inorder_traverse(node->left, result);
    result.push_back(node->data);
    inorder_traverse(node->right, result);
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::preorder_traverse(TreeNode<T>* node, std::vector<T>& result) const {
    if (!node) return;
    result.push_back(node->data);
    preorder_traverse(node->left, result);
    preorder_traverse(node->right, result);
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::postorder_traverse(TreeNode<T>* node, std::vector<T>& result) const {
    if (!node) return;
    postorder_traverse(node->left, result);
    postorder_traverse(node->right, result);
    result.push_back(node->data);
}

template <typename T, typename Compare>
std::vector<T> BinarySearchTree<T, Compare>::inorder() const {
    std::vector<T> result;
    result.reserve(size_);
    inorder_traverse(root_, result);
    return result;
}

template <typename T, typename Compare>
std::vector<T> BinarySearchTree<T, Compare>::preorder() const {
    std::vector<T> result;
    result.reserve(size_);
    preorder_traverse(root_, result);
    return result;
}

template <typename T, typename Compare>
std::vector<T> BinarySearchTree<T, Compare>::postorder() const {
    std::vector<T> result;
    result.reserve(size_);
    postorder_traverse(root_, result);
    return result;
}

template <typename T, typename Compare>
std::vector<T> BinarySearchTree<T, Compare>::level_order() const {
    std::vector<T> result;
    if (!root_) return result;

    std::queue<TreeNode<T>*> q;
    q.push(root_);

    while (!q.empty()) {
        TreeNode<T>* node = q.front();
        q.pop();
        result.push_back(node->data);

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    return result;
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::inorder_visit(const std::function<void(const T&)>& visitor) const {
    std::function<void(TreeNode<T>*)> visit = [&](TreeNode<T>* node) {
        if (!node) return;
        visit(node->left);
        visitor(node->data);
        visit(node->right);
    };
    visit(root_);
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::preorder_visit(const std::function<void(const T&)>& visitor) const {
    std::function<void(TreeNode<T>*)> visit = [&](TreeNode<T>* node) {
        if (!node) return;
        visitor(node->data);
        visit(node->left);
        visit(node->right);
    };
    visit(root_);
}

template <typename T, typename Compare>
void BinarySearchTree<T, Compare>::postorder_visit(const std::function<void(const T&)>& visitor) const {
    std::function<void(TreeNode<T>*)> visit = [&](TreeNode<T>* node) {
        if (!node) return;
        visit(node->left);
        visit(node->right);
        visitor(node->data);
    };
    visit(root_);
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::kth_smallest(size_t k) const {
    if (k == 0 || k > size_) return std::nullopt;

    std::vector<T> sorted = inorder();
    return sorted[k - 1];
}

template <typename T, typename Compare>
std::optional<T> BinarySearchTree<T, Compare>::kth_largest(size_t k) const {
    if (k == 0 || k > size_) return std::nullopt;

    std::vector<T> sorted = inorder();
    return sorted[size_ - k];
}

// =============================================================================
// Trie
// =============================================================================

Trie::Trie() : word_count_(0), node_count_(1) {
    root_ = new TrieNode();
}

Trie::~Trie() {
    delete_tree(root_);
}

void Trie::delete_tree(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; ++i) {
        delete_tree(node->children[i]);
    }
    delete node;
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root_;
    for (char c : word) {
        int index = c - 'a';
        if (index < 0 || index >= 26) continue;  // Skip non-lowercase

        if (!current->children[index]) {
            current->children[index] = new TrieNode();
            ++node_count_;
        }
        current = current->children[index];
        current->count++;
    }

    if (!current->is_end) {
        current->is_end = true;
        ++word_count_;
    }
}

bool Trie::search(const std::string& word) const {
    TrieNode* node = find_node(word);
    return node && node->is_end;
}

bool Trie::starts_with(const std::string& prefix) const {
    return find_node(prefix) != nullptr;
}

Trie::TrieNode* Trie::find_node(const std::string& prefix) const {
    TrieNode* current = root_;
    for (char c : prefix) {
        int index = c - 'a';
        if (index < 0 || index >= 26) return nullptr;
        if (!current->children[index]) return nullptr;
        current = current->children[index];
    }
    return current;
}

bool Trie::remove_helper(TrieNode* node, const std::string& word, size_t depth) {
    if (!node) return false;

    if (depth == word.size()) {
        if (!node->is_end) return false;
        node->is_end = false;
        --word_count_;

        // Return true if node has no children
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) return false;
        }
        return true;
    }

    int index = word[depth] - 'a';
    if (index < 0 || index >= 26) return false;

    if (remove_helper(node->children[index], word, depth + 1)) {
        delete node->children[index];
        node->children[index] = nullptr;
        --node_count_;

        // Return true if this node should also be deleted
        if (!node->is_end) {
            for (int i = 0; i < 26; ++i) {
                if (node->children[i]) return false;
            }
            return true;
        }
    }

    return false;
}

bool Trie::remove(const std::string& word) {
    if (!search(word)) return false;
    remove_helper(root_, word, 0);
    return true;
}

size_t Trie::word_count() const {
    return word_count_;
}

size_t Trie::node_count() const {
    return node_count_;
}

bool Trie::empty() const {
    return word_count_ == 0;
}

void Trie::clear() {
    delete_tree(root_);
    root_ = new TrieNode();
    word_count_ = 0;
    node_count_ = 1;
}

void Trie::collect_words(TrieNode* node, const std::string& prefix,
                         std::vector<std::string>& words) const {
    if (!node) return;

    if (node->is_end) {
        words.push_back(prefix);
    }

    for (int i = 0; i < 26; ++i) {
        if (node->children[i]) {
            collect_words(node->children[i], prefix + char('a' + i), words);
        }
    }
}

std::vector<std::string> Trie::get_all_words() const {
    std::vector<std::string> words;
    collect_words(root_, "", words);
    return words;
}

std::vector<std::string> Trie::get_words_with_prefix(const std::string& prefix) const {
    std::vector<std::string> words;
    TrieNode* node = find_node(prefix);
    if (node) {
        collect_words(node, prefix, words);
    }
    return words;
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix, size_t max_results) const {
    std::vector<std::string> all_words = get_words_with_prefix(prefix);
    if (all_words.size() > max_results) {
        all_words.resize(max_results);
    }
    return all_words;
}

// Explicit template instantiations
template class BinarySearchTree<int>;
template class BinarySearchTree<double>;
template class BinarySearchTree<std::string>;

}  // namespace temp2::containers
