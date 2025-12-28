#ifndef TEMP2_MATH_BASIC_MATH_HPP
#define TEMP2_MATH_BASIC_MATH_HPP

#include <cstdint>

namespace temp2::math {

/**
 * @brief Basic arithmetic operations
 */
class BasicMath {
public:
    // Arithmetic operations
    static int add(int a, int b);
    static int subtract(int a, int b);
    static int multiply(int a, int b);
    static double divide(double a, double b);
    static int modulo(int a, int b);

    // Power and roots
    static double power(double base, int exponent);
    static double square_root(double value);
    static double cube_root(double value);
    static double nth_root(double value, int n);

    // Absolute and sign
    static int abs(int value);
    static double abs(double value);
    static int sign(int value);
    static int sign(double value);

    // Min/Max
    static int min(int a, int b);
    static int max(int a, int b);
    static double min(double a, double b);
    static double max(double a, double b);

    // Clamping
    static int clamp(int value, int min_val, int max_val);
    static double clamp(double value, double min_val, double max_val);
};

/**
 * @brief Integer-specific operations
 */
class IntegerMath {
public:
    // GCD and LCM
    static int gcd(int a, int b);
    static int lcm(int a, int b);

    // Factorial and combinations
    static uint64_t factorial(int n);
    static uint64_t permutations(int n, int r);
    static uint64_t combinations(int n, int r);

    // Prime operations
    static bool is_prime(int n);
    static int next_prime(int n);
    static int nth_prime(int n);

    // Fibonacci
    static uint64_t fibonacci(int n);
    static uint64_t fibonacci_iterative(int n);

    // Bit operations
    static int count_bits(int n);
    static int highest_bit(int n);
    static int lowest_bit(int n);
    static bool is_power_of_two(int n);
};

/**
 * @brief Rounding operations
 */
class Rounding {
public:
    static int floor(double value);
    static int ceil(double value);
    static int round(double value);
    static double round_to_decimals(double value, int decimals);
    static int truncate(double value);
};

}  // namespace temp2::math

#endif  // TEMP2_MATH_BASIC_MATH_HPP
