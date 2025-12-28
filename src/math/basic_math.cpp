#include "math/basic_math.hpp"
#include <cmath>
#include <stdexcept>

namespace temp2::math {

// =============================================================================
// BasicMath
// =============================================================================

int BasicMath::add(int a, int b) {
    return a + b;
}

int BasicMath::subtract(int a, int b) {
    return a - b;
}

int BasicMath::multiply(int a, int b) {
    return a * b;
}

double BasicMath::divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int BasicMath::modulo(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Modulo by zero");
    }
    return a % b;
}

double BasicMath::power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }

    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

double BasicMath::square_root(double value) {
    if (value < 0) {
        throw std::invalid_argument("Square root of negative number");
    }
    return std::sqrt(value);
}

double BasicMath::cube_root(double value) {
    return std::cbrt(value);
}

double BasicMath::nth_root(double value, int n) {
    if (n == 0) {
        throw std::invalid_argument("Zeroth root undefined");
    }
    if (value < 0 && n % 2 == 0) {
        throw std::invalid_argument("Even root of negative number");
    }
    return std::pow(std::abs(value), 1.0 / n) * (value < 0 ? -1 : 1);
}

int BasicMath::abs(int value) {
    return value < 0 ? -value : value;
}

double BasicMath::abs(double value) {
    return value < 0 ? -value : value;
}

int BasicMath::sign(int value) {
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}

int BasicMath::sign(double value) {
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}

int BasicMath::min(int a, int b) {
    return a < b ? a : b;
}

int BasicMath::max(int a, int b) {
    return a > b ? a : b;
}

double BasicMath::min(double a, double b) {
    return a < b ? a : b;
}

double BasicMath::max(double a, double b) {
    return a > b ? a : b;
}

int BasicMath::clamp(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

double BasicMath::clamp(double value, double min_val, double max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// =============================================================================
// IntegerMath
// =============================================================================

int IntegerMath::gcd(int a, int b) {
    a = BasicMath::abs(a);
    b = BasicMath::abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int IntegerMath::lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return BasicMath::abs(a * b) / gcd(a, b);
}

uint64_t IntegerMath::factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial of negative number");
    }
    uint64_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

uint64_t IntegerMath::permutations(int n, int r) {
    if (n < 0 || r < 0 || r > n) {
        throw std::invalid_argument("Invalid permutation arguments");
    }
    uint64_t result = 1;
    for (int i = n; i > n - r; --i) {
        result *= i;
    }
    return result;
}

uint64_t IntegerMath::combinations(int n, int r) {
    if (n < 0 || r < 0 || r > n) {
        throw std::invalid_argument("Invalid combination arguments");
    }
    if (r > n - r) {
        r = n - r;
    }
    uint64_t result = 1;
    for (int i = 0; i < r; ++i) {
        result = result * (n - i) / (i + 1);
    }
    return result;
}

bool IntegerMath::is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

int IntegerMath::next_prime(int n) {
    if (n < 2) return 2;
    int candidate = n + 1;
    while (!is_prime(candidate)) {
        ++candidate;
    }
    return candidate;
}

int IntegerMath::nth_prime(int n) {
    if (n <= 0) {
        throw std::invalid_argument("n must be positive");
    }
    int count = 0;
    int candidate = 1;
    while (count < n) {
        ++candidate;
        if (is_prime(candidate)) {
            ++count;
        }
    }
    return candidate;
}

uint64_t IntegerMath::fibonacci(int n) {
    if (n < 0) {
        throw std::invalid_argument("Fibonacci of negative number");
    }
    if (n <= 1) return n;

    // Matrix exponentiation for O(log n)
    uint64_t a = 1, b = 0, c = 0, d = 1;
    uint64_t temp_a, temp_b;

    while (n > 0) {
        if (n & 1) {
            temp_a = d * b + c * a;
            temp_b = d * (b + a) + c * b;
            a = temp_a;
            b = temp_b;
        }
        temp_a = c * c + d * d;
        temp_b = d * (2 * c + d);
        c = temp_a;
        d = temp_b;
        n >>= 1;
    }
    return a + b;
}

uint64_t IntegerMath::fibonacci_iterative(int n) {
    if (n < 0) {
        throw std::invalid_argument("Fibonacci of negative number");
    }
    if (n <= 1) return n;

    uint64_t prev = 0, curr = 1;
    for (int i = 2; i <= n; ++i) {
        uint64_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

int IntegerMath::count_bits(int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int IntegerMath::highest_bit(int n) {
    if (n == 0) return -1;
    int bit = 0;
    while (n >>= 1) {
        ++bit;
    }
    return bit;
}

int IntegerMath::lowest_bit(int n) {
    if (n == 0) return -1;
    int bit = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        ++bit;
    }
    return bit;
}

bool IntegerMath::is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// =============================================================================
// Rounding
// =============================================================================

int Rounding::floor(double value) {
    return static_cast<int>(std::floor(value));
}

int Rounding::ceil(double value) {
    return static_cast<int>(std::ceil(value));
}

int Rounding::round(double value) {
    return static_cast<int>(std::round(value));
}

double Rounding::round_to_decimals(double value, int decimals) {
    double multiplier = BasicMath::power(10.0, decimals);
    return std::round(value * multiplier) / multiplier;
}

int Rounding::truncate(double value) {
    return static_cast<int>(value);
}

}  // namespace temp2::math
