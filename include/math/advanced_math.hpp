#ifndef TEMP2_MATH_ADVANCED_MATH_HPP
#define TEMP2_MATH_ADVANCED_MATH_HPP

#include <vector>
#include <cmath>

namespace temp2::math {

/**
 * @brief Trigonometric functions
 */
class Trigonometry {
public:
    // Basic trig
    static double sin(double radians);
    static double cos(double radians);
    static double tan(double radians);

    // Inverse trig
    static double asin(double value);
    static double acos(double value);
    static double atan(double value);
    static double atan2(double y, double x);

    // Hyperbolic
    static double sinh(double value);
    static double cosh(double value);
    static double tanh(double value);

    // Conversions
    static double degrees_to_radians(double degrees);
    static double radians_to_degrees(double radians);

    // Constants
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double TAU = 2.0 * PI;
    static constexpr double E = 2.71828182845904523536;
};

/**
 * @brief Exponential and logarithmic functions
 */
class Exponential {
public:
    static double exp(double value);
    static double exp2(double value);
    static double expm1(double value);  // exp(x) - 1

    static double log(double value);      // natural log
    static double log10(double value);    // base 10
    static double log2(double value);     // base 2
    static double log1p(double value);    // log(1 + x)
    static double log_base(double value, double base);
};

/**
 * @brief Complex number operations
 */
class Complex {
public:
    double real;
    double imag;

    Complex();
    Complex(double r, double i);

    // Arithmetic
    Complex add(const Complex& other) const;
    Complex subtract(const Complex& other) const;
    Complex multiply(const Complex& other) const;
    Complex divide(const Complex& other) const;

    // Properties
    double magnitude() const;
    double phase() const;
    Complex conjugate() const;

    // Conversion
    static Complex from_polar(double magnitude, double phase);
    void to_polar(double& magnitude, double& phase) const;
};

/**
 * @brief Matrix operations (2x2 and 3x3)
 */
class Matrix2x2 {
public:
    double data[2][2];

    Matrix2x2();
    Matrix2x2(double a, double b, double c, double d);

    // Operations
    Matrix2x2 add(const Matrix2x2& other) const;
    Matrix2x2 subtract(const Matrix2x2& other) const;
    Matrix2x2 multiply(const Matrix2x2& other) const;
    Matrix2x2 scale(double scalar) const;

    // Properties
    double determinant() const;
    double trace() const;
    Matrix2x2 transpose() const;
    Matrix2x2 inverse() const;

    // Special matrices
    static Matrix2x2 identity();
    static Matrix2x2 rotation(double radians);
    static Matrix2x2 scaling(double sx, double sy);
};

class Matrix3x3 {
public:
    double data[3][3];

    Matrix3x3();

    Matrix3x3 add(const Matrix3x3& other) const;
    Matrix3x3 multiply(const Matrix3x3& other) const;
    double determinant() const;
    Matrix3x3 transpose() const;
    Matrix3x3 inverse() const;

    static Matrix3x3 identity();
    static Matrix3x3 rotation_x(double radians);
    static Matrix3x3 rotation_y(double radians);
    static Matrix3x3 rotation_z(double radians);
};

/**
 * @brief Interpolation functions
 */
class Interpolation {
public:
    // Linear
    static double lerp(double a, double b, double t);
    static double inverse_lerp(double a, double b, double value);
    static double remap(double value, double from_min, double from_max,
                        double to_min, double to_max);

    // Smooth interpolation
    static double smoothstep(double edge0, double edge1, double x);
    static double smootherstep(double edge0, double edge1, double x);

    // Easing functions
    static double ease_in_quad(double t);
    static double ease_out_quad(double t);
    static double ease_in_out_quad(double t);
    static double ease_in_cubic(double t);
    static double ease_out_cubic(double t);
    static double ease_in_out_cubic(double t);

    // Bezier
    static double bezier_quad(double p0, double p1, double p2, double t);
    static double bezier_cubic(double p0, double p1, double p2, double p3, double t);
};

/**
 * @brief Polynomial operations
 */
class Polynomial {
public:
    std::vector<double> coefficients;

    Polynomial();
    explicit Polynomial(const std::vector<double>& coeffs);

    double evaluate(double x) const;
    Polynomial derivative() const;
    Polynomial integral(double constant = 0.0) const;

    Polynomial add(const Polynomial& other) const;
    Polynomial subtract(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;

    int degree() const;
};

}  // namespace temp2::math

#endif  // TEMP2_MATH_ADVANCED_MATH_HPP
