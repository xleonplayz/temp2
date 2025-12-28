#ifndef TEMP2_ALGORITHMS_SEARCHING_HPP
#define TEMP2_ALGORITHMS_SEARCHING_HPP

#include <vector>
#include <optional>
#include <functional>

namespace temp2::algorithms {

/**
 * @brief Searching algorithms collection
 */
class Searching {
public:
    // Linear search
    template <typename T>
    static std::optional<size_t> linear_search(const std::vector<T>& arr, const T& target);

    template <typename T, typename Predicate>
    static std::optional<size_t> find_if(const std::vector<T>& arr, Predicate pred);

    // Binary search (requires sorted array)
    template <typename T>
    static std::optional<size_t> binary_search(const std::vector<T>& arr, const T& target);

    template <typename T, typename Compare>
    static std::optional<size_t> binary_search(const std::vector<T>& arr, const T& target, Compare comp);

    // Bounds
    template <typename T>
    static size_t lower_bound(const std::vector<T>& arr, const T& target);

    template <typename T>
    static size_t upper_bound(const std::vector<T>& arr, const T& target);

    template <typename T>
    static std::pair<size_t, size_t> equal_range(const std::vector<T>& arr, const T& target);

    // Interpolation search (for uniformly distributed data)
    template <typename T>
    static std::optional<size_t> interpolation_search(const std::vector<T>& arr, const T& target);

    // Jump search
    template <typename T>
    static std::optional<size_t> jump_search(const std::vector<T>& arr, const T& target);

    // Exponential search
    template <typename T>
    static std::optional<size_t> exponential_search(const std::vector<T>& arr, const T& target);

    // Ternary search (for unimodal functions)
    template <typename T>
    static size_t ternary_search_max(const std::vector<T>& arr);

    template <typename T>
    static size_t ternary_search_min(const std::vector<T>& arr);

    // Find extremes
    template <typename T>
    static std::optional<size_t> find_min(const std::vector<T>& arr);

    template <typename T>
    static std::optional<size_t> find_max(const std::vector<T>& arr);

    template <typename T>
    static std::pair<std::optional<size_t>, std::optional<size_t>> find_min_max(const std::vector<T>& arr);

    // K-th element
    template <typename T>
    static T kth_smallest(std::vector<T> arr, size_t k);

    template <typename T>
    static T kth_largest(std::vector<T> arr, size_t k);

    // Count occurrences
    template <typename T>
    static size_t count(const std::vector<T>& arr, const T& target);

    template <typename T, typename Predicate>
    static size_t count_if(const std::vector<T>& arr, Predicate pred);

private:
    template <typename T>
    static T quick_select(std::vector<T>& arr, size_t left, size_t right, size_t k);
};

}  // namespace temp2::algorithms

#endif  // TEMP2_ALGORITHMS_SEARCHING_HPP
