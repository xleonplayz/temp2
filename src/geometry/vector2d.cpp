#include "geometry/vector2d.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace temp2::geometry {

// =============================================================================
// Vector2D
// =============================================================================

Vector2D::Vector2D() : x(0), y(0) {}
Vector2D::Vector2D(double x, double y) : x(x), y(y) {}
Vector2D::Vector2D(const Point2D& point) : x(point.x), y(point.y) {}
Vector2D::Vector2D(const Point2D& from, const Point2D& to)
    : x(to.x - from.x), y(to.y - from.y) {}

Vector2D Vector2D::operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
Vector2D Vector2D::operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
Vector2D Vector2D::operator*(double scalar) const { return Vector2D(x * scalar, y * scalar); }
Vector2D Vector2D::operator/(double scalar) const { return Vector2D(x / scalar, y / scalar); }

Vector2D& Vector2D::operator+=(const Vector2D& other) { x += other.x; y += other.y; return *this; }
Vector2D& Vector2D::operator-=(const Vector2D& other) { x -= other.x; y -= other.y; return *this; }
Vector2D& Vector2D::operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
Vector2D& Vector2D::operator/=(double scalar) { x /= scalar; y /= scalar; return *this; }

Vector2D Vector2D::operator-() const { return Vector2D(-x, -y); }

bool Vector2D::operator==(const Vector2D& other) const { return x == other.x && y == other.y; }
bool Vector2D::operator!=(const Vector2D& other) const { return !(*this == other); }

double Vector2D::length() const { return std::sqrt(x * x + y * y); }
double Vector2D::length_squared() const { return x * x + y * y; }
Vector2D Vector2D::normalized() const {
    double len = length();
    return len == 0 ? Vector2D() : Vector2D(x / len, y / len);
}
void Vector2D::normalize() { *this = normalized(); }

double Vector2D::dot(const Vector2D& other) const { return x * other.x + y * other.y; }
double Vector2D::cross(const Vector2D& other) const { return x * other.y - y * other.x; }

double Vector2D::angle() const { return std::atan2(y, x); }
double Vector2D::angle_between(const Vector2D& other) const {
    double d = dot(other) / (length() * other.length());
    return std::acos(std::max(-1.0, std::min(1.0, d)));
}
bool Vector2D::is_parallel_to(const Vector2D& other, double epsilon) const {
    return std::abs(cross(other)) < epsilon;
}
bool Vector2D::is_perpendicular_to(const Vector2D& other, double epsilon) const {
    return std::abs(dot(other)) < epsilon;
}

Vector2D Vector2D::rotated(double radians) const {
    double c = std::cos(radians), s = std::sin(radians);
    return Vector2D(x * c - y * s, x * s + y * c);
}
Vector2D Vector2D::perpendicular() const { return Vector2D(-y, x); }
Vector2D Vector2D::reflected(const Vector2D& normal) const {
    return *this - normal * (2.0 * dot(normal));
}
Vector2D Vector2D::projected_onto(const Vector2D& other) const {
    double d = other.dot(other);
    return d == 0 ? Vector2D() : other * (dot(other) / d);
}
Vector2D Vector2D::rejected_from(const Vector2D& other) const {
    return *this - projected_onto(other);
}

Vector2D Vector2D::lerp(const Vector2D& target, double t) const {
    return Vector2D(x + (target.x - x) * t, y + (target.y - y) * t);
}
Vector2D Vector2D::slerp(const Vector2D& target, double t) const {
    double angle_val = angle_between(target);
    if (std::abs(angle_val) < 1e-9) return lerp(target, t);
    double s = std::sin(angle_val);
    return *this * (std::sin((1 - t) * angle_val) / s) + target * (std::sin(t * angle_val) / s);
}

std::string Vector2D::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << "[" << x << ", " << y << "]";
    return oss.str();
}
bool Vector2D::is_zero() const { return x == 0 && y == 0; }
bool Vector2D::near_equal(const Vector2D& other, double epsilon) const {
    return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
}
Point2D Vector2D::to_point() const { return Point2D(x, y); }

Vector2D Vector2D::from_angle(double radians) { return Vector2D(std::cos(radians), std::sin(radians)); }
Vector2D Vector2D::zero() { return Vector2D(0, 0); }
Vector2D Vector2D::up() { return Vector2D(0, 1); }
Vector2D Vector2D::down() { return Vector2D(0, -1); }
Vector2D Vector2D::left() { return Vector2D(-1, 0); }
Vector2D Vector2D::right() { return Vector2D(1, 0); }

// =============================================================================
// Vector3D
// =============================================================================

Vector3D::Vector3D() : x(0), y(0), z(0) {}
Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
Vector3D::Vector3D(const Point3D& point) : x(point.x), y(point.y), z(point.z) {}
Vector3D::Vector3D(const Point3D& from, const Point3D& to)
    : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z) {}

Vector3D Vector3D::operator+(const Vector3D& other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
Vector3D Vector3D::operator-(const Vector3D& other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
Vector3D Vector3D::operator*(double scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
Vector3D Vector3D::operator/(double scalar) const { return Vector3D(x / scalar, y / scalar, z / scalar); }

Vector3D& Vector3D::operator+=(const Vector3D& other) { x += other.x; y += other.y; z += other.z; return *this; }
Vector3D& Vector3D::operator-=(const Vector3D& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
Vector3D& Vector3D::operator*=(double scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
Vector3D& Vector3D::operator/=(double scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

Vector3D Vector3D::operator-() const { return Vector3D(-x, -y, -z); }

bool Vector3D::operator==(const Vector3D& other) const { return x == other.x && y == other.y && z == other.z; }
bool Vector3D::operator!=(const Vector3D& other) const { return !(*this == other); }

double Vector3D::length() const { return std::sqrt(x * x + y * y + z * z); }
double Vector3D::length_squared() const { return x * x + y * y + z * z; }
Vector3D Vector3D::normalized() const {
    double len = length();
    return len == 0 ? Vector3D() : Vector3D(x / len, y / len, z / len);
}
void Vector3D::normalize() { *this = normalized(); }

double Vector3D::dot(const Vector3D& other) const { return x * other.x + y * other.y + z * other.z; }
Vector3D Vector3D::cross(const Vector3D& other) const {
    return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}
double Vector3D::scalar_triple(const Vector3D& b, const Vector3D& c) const { return dot(b.cross(c)); }

double Vector3D::angle_between(const Vector3D& other) const {
    double d = dot(other) / (length() * other.length());
    return std::acos(std::max(-1.0, std::min(1.0, d)));
}
bool Vector3D::is_parallel_to(const Vector3D& other, double epsilon) const {
    return cross(other).length() < epsilon;
}
bool Vector3D::is_perpendicular_to(const Vector3D& other, double epsilon) const {
    return std::abs(dot(other)) < epsilon;
}

Vector3D Vector3D::rotated_around_x(double radians) const {
    double c = std::cos(radians), s = std::sin(radians);
    return Vector3D(x, y * c - z * s, y * s + z * c);
}
Vector3D Vector3D::rotated_around_y(double radians) const {
    double c = std::cos(radians), s = std::sin(radians);
    return Vector3D(x * c + z * s, y, -x * s + z * c);
}
Vector3D Vector3D::rotated_around_z(double radians) const {
    double c = std::cos(radians), s = std::sin(radians);
    return Vector3D(x * c - y * s, x * s + y * c, z);
}
Vector3D Vector3D::rotated_around_axis(const Vector3D& axis, double radians) const {
    Vector3D k = axis.normalized();
    double c = std::cos(radians), s = std::sin(radians);
    return *this * c + k.cross(*this) * s + k * k.dot(*this) * (1 - c);
}
Vector3D Vector3D::projected_onto(const Vector3D& other) const {
    double d = other.dot(other);
    return d == 0 ? Vector3D() : other * (dot(other) / d);
}
Vector3D Vector3D::projected_onto_plane(const Vector3D& normal) const {
    return *this - projected_onto(normal);
}
Vector3D Vector3D::reflected(const Vector3D& normal) const {
    return *this - normal * (2.0 * dot(normal));
}
Vector3D Vector3D::lerp(const Vector3D& target, double t) const {
    return Vector3D(x + (target.x - x) * t, y + (target.y - y) * t, z + (target.z - z) * t);
}

std::string Vector3D::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << "[" << x << ", " << y << ", " << z << "]";
    return oss.str();
}
bool Vector3D::is_zero() const { return x == 0 && y == 0 && z == 0; }
bool Vector3D::near_equal(const Vector3D& other, double epsilon) const {
    return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon && std::abs(z - other.z) < epsilon;
}
Point3D Vector3D::to_point() const { return Point3D(x, y, z); }

Vector3D Vector3D::zero() { return Vector3D(0, 0, 0); }
Vector3D Vector3D::unit_x() { return Vector3D(1, 0, 0); }
Vector3D Vector3D::unit_y() { return Vector3D(0, 1, 0); }
Vector3D Vector3D::unit_z() { return Vector3D(0, 0, 1); }
Vector3D Vector3D::up() { return Vector3D(0, 1, 0); }
Vector3D Vector3D::forward() { return Vector3D(0, 0, -1); }
Vector3D Vector3D::right() { return Vector3D(1, 0, 0); }

}  // namespace temp2::geometry
