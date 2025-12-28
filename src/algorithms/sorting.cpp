#include "algorithms/sorting.hpp"
#include <algorithm>

namespace temp2::algorithms {

template <typename T, typename Compare>
void Sorting::bubble_sort(std::vector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (comp(arr[j + 1], arr[j])) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template <typename T, typename Compare>
void Sorting::selection_sort(std::vector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (comp(arr[j], arr[min_idx])) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}

template <typename T, typename Compare>
void Sorting::insertion_sort(std::vector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; ++i) {
        T key = arr[i];
        size_t j = i;
        while (j > 0 && comp(key, arr[j - 1])) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

template <typename T, typename Compare>
void Sorting::merge(std::vector<T>& arr, size_t left, size_t mid, size_t right, Compare comp) {
    std::vector<T> temp(right - left + 1);
    size_t i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (comp(arr[i], arr[j]) || !comp(arr[j], arr[i])) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (size_t i = 0; i < temp.size(); ++i) {
        arr[left + i] = temp[i];
    }
}

template <typename T, typename Compare>
void Sorting::merge_sort_recursive(std::vector<T>& arr, size_t left, size_t right, Compare comp) {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    merge_sort_recursive(arr, left, mid, comp);
    merge_sort_recursive(arr, mid + 1, right, comp);
    merge(arr, left, mid, right, comp);
}

template <typename T, typename Compare>
void Sorting::merge_sort(std::vector<T>& arr, Compare comp) {
    if (arr.size() <= 1) return;
    merge_sort_recursive(arr, 0, arr.size() - 1, comp);
}

template <typename T, typename Compare>
size_t Sorting::partition(std::vector<T>& arr, size_t left, size_t right, Compare comp) {
    T pivot = arr[right];
    size_t i = left;
    for (size_t j = left; j < right; ++j) {
        if (comp(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[right]);
    return i;
}

template <typename T, typename Compare>
void Sorting::quick_sort_recursive(std::vector<T>& arr, size_t left, size_t right, Compare comp) {
    if (left >= right) return;
    size_t pivot = partition(arr, left, right, comp);
    if (pivot > 0) quick_sort_recursive(arr, left, pivot - 1, comp);
    quick_sort_recursive(arr, pivot + 1, right, comp);
}

template <typename T, typename Compare>
void Sorting::quick_sort(std::vector<T>& arr, Compare comp) {
    if (arr.size() <= 1) return;
    quick_sort_recursive(arr, 0, arr.size() - 1, comp);
}

template <typename T, typename Compare>
void Sorting::heapify(std::vector<T>& arr, size_t n, size_t i, Compare comp) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n && comp(arr[largest], arr[left])) {
        largest = left;
    }
    if (right < n && comp(arr[largest], arr[right])) {
        largest = right;
    }
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest, comp);
    }
}

template <typename T, typename Compare>
void Sorting::heap_sort(std::vector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i, comp);
    }
    for (size_t i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0, comp);
    }
}

template <typename T>
bool Sorting::is_sorted(const std::vector<T>& arr) {
    return is_sorted(arr, std::less<T>());
}

template <typename T, typename Compare>
bool Sorting::is_sorted(const std::vector<T>& arr, Compare comp) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (comp(arr[i], arr[i - 1])) return false;
    }
    return true;
}

// Explicit instantiations
template void Sorting::bubble_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::bubble_sort<double>(std::vector<double>&, std::less<double>);
template void Sorting::selection_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::selection_sort<double>(std::vector<double>&, std::less<double>);
template void Sorting::insertion_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::insertion_sort<double>(std::vector<double>&, std::less<double>);
template void Sorting::merge_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::merge_sort<double>(std::vector<double>&, std::less<double>);
template void Sorting::quick_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::quick_sort<double>(std::vector<double>&, std::less<double>);
template void Sorting::heap_sort<int>(std::vector<int>&, std::less<int>);
template void Sorting::heap_sort<double>(std::vector<double>&, std::less<double>);
template bool Sorting::is_sorted<int>(const std::vector<int>&);
template bool Sorting::is_sorted<double>(const std::vector<double>&);

}  // namespace temp2::algorithms
