#ifndef TEMP2_ALGORITHMS_SORTING_HPP
#define TEMP2_ALGORITHMS_SORTING_HPP

#include <vector>
#include <functional>

namespace temp2::algorithms {

/**
 * @brief Sorting algorithms collection
 */
class Sorting {
public:
    // Basic sorts
    template <typename T, typename Compare = std::less<T>>
    static void bubble_sort(std::vector<T>& arr, Compare comp = Compare());

    template <typename T, typename Compare = std::less<T>>
    static void selection_sort(std::vector<T>& arr, Compare comp = Compare());

    template <typename T, typename Compare = std::less<T>>
    static void insertion_sort(std::vector<T>& arr, Compare comp = Compare());

    // Efficient sorts
    template <typename T, typename Compare = std::less<T>>
    static void merge_sort(std::vector<T>& arr, Compare comp = Compare());

    template <typename T, typename Compare = std::less<T>>
    static void quick_sort(std::vector<T>& arr, Compare comp = Compare());

    template <typename T, typename Compare = std::less<T>>
    static void heap_sort(std::vector<T>& arr, Compare comp = Compare());

    // Special sorts
    template <typename T>
    static void counting_sort(std::vector<T>& arr);

    template <typename T>
    static void radix_sort(std::vector<T>& arr);

    // Partial sorts
    template <typename T, typename Compare = std::less<T>>
    static void partial_sort(std::vector<T>& arr, size_t k, Compare comp = Compare());

    // Utility
    template <typename T>
    static bool is_sorted(const std::vector<T>& arr);

    template <typename T, typename Compare>
    static bool is_sorted(const std::vector<T>& arr, Compare comp);

private:
    template <typename T, typename Compare>
    static void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right, Compare comp);

    template <typename T, typename Compare>
    static void merge_sort_recursive(std::vector<T>& arr, size_t left, size_t right, Compare comp);

    template <typename T, typename Compare>
    static size_t partition(std::vector<T>& arr, size_t left, size_t right, Compare comp);

    template <typename T, typename Compare>
    static void quick_sort_recursive(std::vector<T>& arr, size_t left, size_t right, Compare comp);

    template <typename T, typename Compare>
    static void heapify(std::vector<T>& arr, size_t n, size_t i, Compare comp);
};

}  // namespace temp2::algorithms

#endif  // TEMP2_ALGORITHMS_SORTING_HPP
