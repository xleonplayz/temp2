#include "math/advanced_math.hpp"
#include <stdexcept>
#include <cmath>

namespace temp2::math {

// =============================================================================
// Trigonometry
// =============================================================================

double Trigonometry::sin(double radians) {
    return std::sin(radians);
}

double Trigonometry::cos(double radians) {
    return std::cos(radians);
}

double Trigonometry::tan(double radians) {
    return std::tan(radians);
}

double Trigonometry::asin(double value) {
    if (value < -1.0 || value > 1.0) {
        throw std::invalid_argument("asin argument out of range [-1, 1]");
    }
    return std::asin(value);
}

double Trigonometry::acos(double value) {
    if (value < -1.0 || value > 1.0) {
        throw std::invalid_argument("acos argument out of range [-1, 1]");
    }
    return std::acos(value);
}

double Trigonometry::atan(double value) {
    return std::atan(value);
}

double Trigonometry::atan2(double y, double x) {
    return std::atan2(y, x);
}

double Trigonometry::sinh(double value) {
    return std::sinh(value);
}

double Trigonometry::cosh(double value) {
    return std::cosh(value);
}

double Trigonometry::tanh(double value) {
    return std::tanh(value);
}

double Trigonometry::degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

double Trigonometry::radians_to_degrees(double radians) {
    return radians * 180.0 / PI;
}

// =============================================================================
// Exponential
// =============================================================================

double Exponential::exp(double value) {
    return std::exp(value);
}

double Exponential::exp2(double value) {
    return std::exp2(value);
}

double Exponential::expm1(double value) {
    return std::expm1(value);
}

double Exponential::log(double value) {
    if (value <= 0) {
        throw std::invalid_argument("Logarithm of non-positive number");
    }
    return std::log(value);
}

double Exponential::log10(double value) {
    if (value <= 0) {
        throw std::invalid_argument("Logarithm of non-positive number");
    }
    return std::log10(value);
}

double Exponential::log2(double value) {
    if (value <= 0) {
        throw std::invalid_argument("Logarithm of non-positive number");
    }
    return std::log2(value);
}

double Exponential::log1p(double value) {
    if (value <= -1) {
        throw std::invalid_argument("log1p argument must be > -1");
    }
    return std::log1p(value);
}

double Exponential::log_base(double value, double base) {
    if (value <= 0 || base <= 0 || base == 1) {
        throw std::invalid_argument("Invalid logarithm arguments");
    }
    return std::log(value) / std::log(base);
}

// =============================================================================
// Complex
// =============================================================================

Complex::Complex() : real(0), imag(0) {}

Complex::Complex(double r, double i) : real(r), imag(i) {}

Complex Complex::add(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::subtract(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::multiply(const Complex& other) const {
    return Complex(
        real * other.real - imag * other.imag,
        real * other.imag + imag * other.real
    );
}

Complex Complex::divide(const Complex& other) const {
    double denom = other.real * other.real + other.imag * other.imag;
    if (denom == 0) {
        throw std::invalid_argument("Division by zero complex number");
    }
    return Complex(
        (real * other.real + imag * other.imag) / denom,
        (imag * other.real - real * other.imag) / denom
    );
}

double Complex::magnitude() const {
    return std::sqrt(real * real + imag * imag);
}

double Complex::phase() const {
    return std::atan2(imag, real);
}

Complex Complex::conjugate() const {
    return Complex(real, -imag);
}

Complex Complex::from_polar(double magnitude, double phase) {
    return Complex(magnitude * std::cos(phase), magnitude * std::sin(phase));
}

void Complex::to_polar(double& mag, double& ph) const {
    mag = magnitude();
    ph = phase();
}

// =============================================================================
// Matrix2x2
// =============================================================================

Matrix2x2::Matrix2x2() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            data[i][j] = 0;
        }
    }
}

Matrix2x2::Matrix2x2(double a, double b, double c, double d) {
    data[0][0] = a; data[0][1] = b;
    data[1][0] = c; data[1][1] = d;
}

Matrix2x2 Matrix2x2::add(const Matrix2x2& other) const {
    Matrix2x2 result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix2x2 Matrix2x2::subtract(const Matrix2x2& other) const {
    Matrix2x2 result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix2x2 Matrix2x2::multiply(const Matrix2x2& other) const {
    Matrix2x2 result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result.data[i][j] = 0;
            for (int k = 0; k < 2; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix2x2 Matrix2x2::scale(double scalar) const {
    Matrix2x2 result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

double Matrix2x2::determinant() const {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

double Matrix2x2::trace() const {
    return data[0][0] + data[1][1];
}

Matrix2x2 Matrix2x2::transpose() const {
    return Matrix2x2(data[0][0], data[1][0], data[0][1], data[1][1]);
}

Matrix2x2 Matrix2x2::inverse() const {
    double det = determinant();
    if (det == 0) {
        throw std::invalid_argument("Matrix is singular");
    }
    return Matrix2x2(
        data[1][1] / det, -data[0][1] / det,
        -data[1][0] / det, data[0][0] / det
    );
}

Matrix2x2 Matrix2x2::identity() {
    return Matrix2x2(1, 0, 0, 1);
}

Matrix2x2 Matrix2x2::rotation(double radians) {
    double c = std::cos(radians);
    double s = std::sin(radians);
    return Matrix2x2(c, -s, s, c);
}

Matrix2x2 Matrix2x2::scaling(double sx, double sy) {
    return Matrix2x2(sx, 0, 0, sy);
}

// =============================================================================
// Matrix3x3
// =============================================================================

Matrix3x3::Matrix3x3() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            data[i][j] = 0;
        }
    }
}

Matrix3x3 Matrix3x3::add(const Matrix3x3& other) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix3x3 Matrix3x3::multiply(const Matrix3x3& other) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.data[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

double Matrix3x3::determinant() const {
    return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1])
         - data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0])
         + data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
}

Matrix3x3 Matrix3x3::transpose() const {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.data[i][j] = data[j][i];
        }
    }
    return result;
}

Matrix3x3 Matrix3x3::inverse() const {
    double det = determinant();
    if (det == 0) {
        throw std::invalid_argument("Matrix is singular");
    }

    Matrix3x3 result;
    result.data[0][0] = (data[1][1] * data[2][2] - data[1][2] * data[2][1]) / det;
    result.data[0][1] = (data[0][2] * data[2][1] - data[0][1] * data[2][2]) / det;
    result.data[0][2] = (data[0][1] * data[1][2] - data[0][2] * data[1][1]) / det;
    result.data[1][0] = (data[1][2] * data[2][0] - data[1][0] * data[2][2]) / det;
    result.data[1][1] = (data[0][0] * data[2][2] - data[0][2] * data[2][0]) / det;
    result.data[1][2] = (data[0][2] * data[1][0] - data[0][0] * data[1][2]) / det;
    result.data[2][0] = (data[1][0] * data[2][1] - data[1][1] * data[2][0]) / det;
    result.data[2][1] = (data[0][1] * data[2][0] - data[0][0] * data[2][1]) / det;
    result.data[2][2] = (data[0][0] * data[1][1] - data[0][1] * data[1][0]) / det;
    return result;
}

Matrix3x3 Matrix3x3::identity() {
    Matrix3x3 result;
    result.data[0][0] = result.data[1][1] = result.data[2][2] = 1;
    return result;
}

Matrix3x3 Matrix3x3::rotation_x(double radians) {
    Matrix3x3 result = identity();
    double c = std::cos(radians);
    double s = std::sin(radians);
    result.data[1][1] = c;  result.data[1][2] = -s;
    result.data[2][1] = s;  result.data[2][2] = c;
    return result;
}

Matrix3x3 Matrix3x3::rotation_y(double radians) {
    Matrix3x3 result = identity();
    double c = std::cos(radians);
    double s = std::sin(radians);
    result.data[0][0] = c;  result.data[0][2] = s;
    result.data[2][0] = -s; result.data[2][2] = c;
    return result;
}

Matrix3x3 Matrix3x3::rotation_z(double radians) {
    Matrix3x3 result = identity();
    double c = std::cos(radians);
    double s = std::sin(radians);
    result.data[0][0] = c;  result.data[0][1] = -s;
    result.data[1][0] = s;  result.data[1][1] = c;
    return result;
}

// =============================================================================
// Interpolation
// =============================================================================

double Interpolation::lerp(double a, double b, double t) {
    return a + t * (b - a);
}

double Interpolation::inverse_lerp(double a, double b, double value) {
    if (a == b) return 0;
    return (value - a) / (b - a);
}

double Interpolation::remap(double value, double from_min, double from_max,
                            double to_min, double to_max) {
    double t = inverse_lerp(from_min, from_max, value);
    return lerp(to_min, to_max, t);
}

double Interpolation::smoothstep(double edge0, double edge1, double x) {
    double t = (x - edge0) / (edge1 - edge0);
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return t * t * (3 - 2 * t);
}

double Interpolation::smootherstep(double edge0, double edge1, double x) {
    double t = (x - edge0) / (edge1 - edge0);
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double Interpolation::ease_in_quad(double t) {
    return t * t;
}

double Interpolation::ease_out_quad(double t) {
    return t * (2 - t);
}

double Interpolation::ease_in_out_quad(double t) {
    return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

double Interpolation::ease_in_cubic(double t) {
    return t * t * t;
}

double Interpolation::ease_out_cubic(double t) {
    double u = t - 1;
    return u * u * u + 1;
}

double Interpolation::ease_in_out_cubic(double t) {
    return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
}

double Interpolation::bezier_quad(double p0, double p1, double p2, double t) {
    double u = 1 - t;
    return u * u * p0 + 2 * u * t * p1 + t * t * p2;
}

double Interpolation::bezier_cubic(double p0, double p1, double p2, double p3, double t) {
    double u = 1 - t;
    return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
}

// =============================================================================
// Polynomial
// =============================================================================

Polynomial::Polynomial() : coefficients{0} {}

Polynomial::Polynomial(const std::vector<double>& coeffs) : coefficients(coeffs) {
    if (coefficients.empty()) {
        coefficients = {0};
    }
}

double Polynomial::evaluate(double x) const {
    // Horner's method
    double result = 0;
    for (int i = coefficients.size() - 1; i >= 0; --i) {
        result = result * x + coefficients[i];
    }
    return result;
}

Polynomial Polynomial::derivative() const {
    if (coefficients.size() <= 1) {
        return Polynomial({0});
    }

    std::vector<double> deriv(coefficients.size() - 1);
    for (size_t i = 1; i < coefficients.size(); ++i) {
        deriv[i - 1] = coefficients[i] * i;
    }
    return Polynomial(deriv);
}

Polynomial Polynomial::integral(double constant) const {
    std::vector<double> integ(coefficients.size() + 1);
    integ[0] = constant;
    for (size_t i = 0; i < coefficients.size(); ++i) {
        integ[i + 1] = coefficients[i] / (i + 1);
    }
    return Polynomial(integ);
}

Polynomial Polynomial::add(const Polynomial& other) const {
    size_t max_size = std::max(coefficients.size(), other.coefficients.size());
    std::vector<double> result(max_size, 0);

    for (size_t i = 0; i < coefficients.size(); ++i) {
        result[i] += coefficients[i];
    }
    for (size_t i = 0; i < other.coefficients.size(); ++i) {
        result[i] += other.coefficients[i];
    }
    return Polynomial(result);
}

Polynomial Polynomial::subtract(const Polynomial& other) const {
    size_t max_size = std::max(coefficients.size(), other.coefficients.size());
    std::vector<double> result(max_size, 0);

    for (size_t i = 0; i < coefficients.size(); ++i) {
        result[i] += coefficients[i];
    }
    for (size_t i = 0; i < other.coefficients.size(); ++i) {
        result[i] -= other.coefficients[i];
    }
    return Polynomial(result);
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    std::vector<double> result(coefficients.size() + other.coefficients.size() - 1, 0);

    for (size_t i = 0; i < coefficients.size(); ++i) {
        for (size_t j = 0; j < other.coefficients.size(); ++j) {
            result[i + j] += coefficients[i] * other.coefficients[j];
        }
    }
    return Polynomial(result);
}

int Polynomial::degree() const {
    for (int i = coefficients.size() - 1; i >= 0; --i) {
        if (coefficients[i] != 0) {
            return i;
        }
    }
    return 0;
}

}  // namespace temp2::math
