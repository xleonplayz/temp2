#include "algorithms/searching.hpp"
#include <algorithm>
#include <cmath>

namespace temp2::algorithms {

template <typename T>
std::optional<size_t> Searching::linear_search(const std::vector<T>& arr, const T& target) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == target) return i;
    }
    return std::nullopt;
}

template <typename T, typename Predicate>
std::optional<size_t> Searching::find_if(const std::vector<T>& arr, Predicate pred) {
    for (size_t i = 0; i < arr.size(); ++i) {
        if (pred(arr[i])) return i;
    }
    return std::nullopt;
}

template <typename T>
std::optional<size_t> Searching::binary_search(const std::vector<T>& arr, const T& target) {
    return binary_search(arr, target, std::less<T>());
}

template <typename T, typename Compare>
std::optional<size_t> Searching::binary_search(const std::vector<T>& arr, const T& target, Compare comp) {
    if (arr.empty()) return std::nullopt;
    size_t left = 0, right = arr.size() - 1;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        if (!comp(arr[mid], target) && !comp(target, arr[mid])) {
            return mid;
        }
        if (comp(arr[mid], target)) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return std::nullopt;
}

template <typename T>
size_t Searching::lower_bound(const std::vector<T>& arr, const T& target) {
    size_t left = 0, right = arr.size();
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

template <typename T>
size_t Searching::upper_bound(const std::vector<T>& arr, const T& target) {
    size_t left = 0, right = arr.size();
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

template <typename T>
std::pair<size_t, size_t> Searching::equal_range(const std::vector<T>& arr, const T& target) {
    return {lower_bound(arr, target), upper_bound(arr, target)};
}

template <typename T>
std::optional<size_t> Searching::jump_search(const std::vector<T>& arr, const T& target) {
    if (arr.empty()) return std::nullopt;
    size_t n = arr.size();
    size_t step = std::sqrt(n);
    size_t prev = 0;

    while (arr[std::min(step, n) - 1] < target) {
        prev = step;
        step += std::sqrt(n);
        if (prev >= n) return std::nullopt;
    }

    for (size_t i = prev; i < std::min(step, n); ++i) {
        if (arr[i] == target) return i;
    }
    return std::nullopt;
}

template <typename T>
std::optional<size_t> Searching::exponential_search(const std::vector<T>& arr, const T& target) {
    if (arr.empty()) return std::nullopt;
    if (arr[0] == target) return 0;

    size_t i = 1;
    while (i < arr.size() && arr[i] <= target) {
        i *= 2;
    }

    size_t left = i / 2;
    size_t right = std::min(i, arr.size() - 1);
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return std::nullopt;
}

template <typename T>
std::optional<size_t> Searching::find_min(const std::vector<T>& arr) {
    if (arr.empty()) return std::nullopt;
    size_t min_idx = 0;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[min_idx]) min_idx = i;
    }
    return min_idx;
}

template <typename T>
std::optional<size_t> Searching::find_max(const std::vector<T>& arr) {
    if (arr.empty()) return std::nullopt;
    size_t max_idx = 0;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > arr[max_idx]) max_idx = i;
    }
    return max_idx;
}

template <typename T>
std::pair<std::optional<size_t>, std::optional<size_t>> Searching::find_min_max(const std::vector<T>& arr) {
    return {find_min(arr), find_max(arr)};
}

template <typename T>
T Searching::quick_select(std::vector<T>& arr, size_t left, size_t right, size_t k) {
    if (left == right) return arr[left];

    size_t pivot = left + (right - left) / 2;
    std::swap(arr[pivot], arr[right]);
    T pivot_val = arr[right];

    size_t store = left;
    for (size_t i = left; i < right; ++i) {
        if (arr[i] < pivot_val) {
            std::swap(arr[i], arr[store]);
            ++store;
        }
    }
    std::swap(arr[store], arr[right]);

    if (k == store) return arr[store];
    if (k < store) return quick_select(arr, left, store - 1, k);
    return quick_select(arr, store + 1, right, k);
}

template <typename T>
T Searching::kth_smallest(std::vector<T> arr, size_t k) {
    return quick_select(arr, 0, arr.size() - 1, k - 1);
}

template <typename T>
T Searching::kth_largest(std::vector<T> arr, size_t k) {
    return quick_select(arr, 0, arr.size() - 1, arr.size() - k);
}

template <typename T>
size_t Searching::count(const std::vector<T>& arr, const T& target) {
    size_t cnt = 0;
    for (const auto& val : arr) {
        if (val == target) ++cnt;
    }
    return cnt;
}

template <typename T, typename Predicate>
size_t Searching::count_if(const std::vector<T>& arr, Predicate pred) {
    size_t cnt = 0;
    for (const auto& val : arr) {
        if (pred(val)) ++cnt;
    }
    return cnt;
}

// Explicit instantiations
template std::optional<size_t> Searching::linear_search<int>(const std::vector<int>&, const int&);
template std::optional<size_t> Searching::linear_search<double>(const std::vector<double>&, const double&);
template std::optional<size_t> Searching::binary_search<int>(const std::vector<int>&, const int&);
template std::optional<size_t> Searching::binary_search<double>(const std::vector<double>&, const double&);
template size_t Searching::lower_bound<int>(const std::vector<int>&, const int&);
template size_t Searching::upper_bound<int>(const std::vector<int>&, const int&);
template std::optional<size_t> Searching::find_min<int>(const std::vector<int>&);
template std::optional<size_t> Searching::find_max<int>(const std::vector<int>&);
template int Searching::kth_smallest<int>(std::vector<int>, size_t);
template int Searching::kth_largest<int>(std::vector<int>, size_t);
template size_t Searching::count<int>(const std::vector<int>&, const int&);

}  // namespace temp2::algorithms
