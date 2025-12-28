#include "geometry/point.hpp"
#include <sstream>
#include <iomanip>

namespace temp2::geometry {

// =============================================================================
// Point2D
// =============================================================================

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(double x, double y) : x(x), y(y) {}

Point2D Point2D::operator+(const Point2D& other) const {
    return Point2D(x + other.x, y + other.y);
}

Point2D Point2D::operator-(const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
}

Point2D Point2D::operator*(double scalar) const {
    return Point2D(x * scalar, y * scalar);
}

Point2D Point2D::operator/(double scalar) const {
    return Point2D(x / scalar, y / scalar);
}

Point2D& Point2D::operator+=(const Point2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point2D& Point2D::operator-=(const Point2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Point2D& Point2D::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Point2D& Point2D::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Point2D Point2D::operator-() const {
    return Point2D(-x, -y);
}

bool Point2D::operator==(const Point2D& other) const {
    return x == other.x && y == other.y;
}

bool Point2D::operator!=(const Point2D& other) const {
    return !(*this == other);
}

double Point2D::distance_to(const Point2D& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

double Point2D::distance_to_origin() const {
    return std::sqrt(x * x + y * y);
}

double Point2D::magnitude() const {
    return distance_to_origin();
}

double Point2D::magnitude_squared() const {
    return x * x + y * y;
}

Point2D Point2D::normalized() const {
    double mag = magnitude();
    if (mag == 0) return Point2D(0, 0);
    return Point2D(x / mag, y / mag);
}

Point2D Point2D::rotated(double radians) const {
    double c = std::cos(radians);
    double s = std::sin(radians);
    return Point2D(x * c - y * s, x * s + y * c);
}

Point2D Point2D::rotated_around(const Point2D& center, double radians) const {
    Point2D translated = *this - center;
    Point2D rotated = translated.rotated(radians);
    return rotated + center;
}

Point2D Point2D::lerp(const Point2D& target, double t) const {
    return Point2D(
        x + (target.x - x) * t,
        y + (target.y - y) * t
    );
}

Point2D Point2D::midpoint(const Point2D& other) const {
    return lerp(other, 0.5);
}

Point2D Point2D::reflected(const Point2D& normal) const {
    double d = 2.0 * dot(normal);
    return Point2D(x - d * normal.x, y - d * normal.y);
}

double Point2D::dot(const Point2D& other) const {
    return x * other.x + y * other.y;
}

double Point2D::cross(const Point2D& other) const {
    return x * other.y - y * other.x;
}

double Point2D::angle() const {
    return std::atan2(y, x);
}

double Point2D::angle_to(const Point2D& other) const {
    return std::atan2(other.y - y, other.x - x);
}

std::string Point2D::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "(" << x << ", " << y << ")";
    return oss.str();
}

bool Point2D::is_zero() const {
    return x == 0 && y == 0;
}

bool Point2D::near_equal(const Point2D& other, double epsilon) const {
    return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
}

Point2D Point2D::from_polar(double radius, double angle) {
    return Point2D(radius * std::cos(angle), radius * std::sin(angle));
}

Point2D Point2D::zero() { return Point2D(0, 0); }
Point2D Point2D::unit_x() { return Point2D(1, 0); }
Point2D Point2D::unit_y() { return Point2D(0, 1); }

// =============================================================================
// Point3D
// =============================================================================

Point3D::Point3D() : x(0), y(0), z(0) {}

Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z) {}

Point3D Point3D::operator+(const Point3D& other) const {
    return Point3D(x + other.x, y + other.y, z + other.z);
}

Point3D Point3D::operator-(const Point3D& other) const {
    return Point3D(x - other.x, y - other.y, z - other.z);
}

Point3D Point3D::operator*(double scalar) const {
    return Point3D(x * scalar, y * scalar, z * scalar);
}

Point3D Point3D::operator/(double scalar) const {
    return Point3D(x / scalar, y / scalar, z / scalar);
}

Point3D& Point3D::operator+=(const Point3D& other) {
    x += other.x; y += other.y; z += other.z;
    return *this;
}

Point3D& Point3D::operator-=(const Point3D& other) {
    x -= other.x; y -= other.y; z -= other.z;
    return *this;
}

Point3D& Point3D::operator*=(double scalar) {
    x *= scalar; y *= scalar; z *= scalar;
    return *this;
}

Point3D& Point3D::operator/=(double scalar) {
    x /= scalar; y /= scalar; z /= scalar;
    return *this;
}

Point3D Point3D::operator-() const {
    return Point3D(-x, -y, -z);
}

bool Point3D::operator==(const Point3D& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Point3D::operator!=(const Point3D& other) const {
    return !(*this == other);
}

double Point3D::distance_to(const Point3D& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    double dz = z - other.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double Point3D::distance_to_origin() const {
    return std::sqrt(x * x + y * y + z * z);
}

double Point3D::magnitude() const {
    return distance_to_origin();
}

double Point3D::magnitude_squared() const {
    return x * x + y * y + z * z;
}

Point3D Point3D::normalized() const {
    double mag = magnitude();
    if (mag == 0) return Point3D(0, 0, 0);
    return Point3D(x / mag, y / mag, z / mag);
}

Point3D Point3D::lerp(const Point3D& target, double t) const {
    return Point3D(
        x + (target.x - x) * t,
        y + (target.y - y) * t,
        z + (target.z - z) * t
    );
}

Point3D Point3D::midpoint(const Point3D& other) const {
    return lerp(other, 0.5);
}

double Point3D::dot(const Point3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Point3D Point3D::cross(const Point3D& other) const {
    return Point3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

Point3D Point3D::project_onto(const Point3D& onto) const {
    double d = onto.dot(onto);
    if (d == 0) return Point3D::zero();
    return onto * (dot(onto) / d);
}

Point3D Point3D::project_onto_plane(const Point3D& normal) const {
    return *this - project_onto(normal);
}

std::string Point3D::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "(" << x << ", " << y << ", " << z << ")";
    return oss.str();
}

bool Point3D::is_zero() const {
    return x == 0 && y == 0 && z == 0;
}

bool Point3D::near_equal(const Point3D& other, double epsilon) const {
    return std::abs(x - other.x) < epsilon &&
           std::abs(y - other.y) < epsilon &&
           std::abs(z - other.z) < epsilon;
}

Point3D Point3D::zero() { return Point3D(0, 0, 0); }
Point3D Point3D::unit_x() { return Point3D(1, 0, 0); }
Point3D Point3D::unit_y() { return Point3D(0, 1, 0); }
Point3D Point3D::unit_z() { return Point3D(0, 0, 1); }

Point3D Point3D::from_spherical(double radius, double theta, double phi) {
    double sin_phi = std::sin(phi);
    return Point3D(
        radius * sin_phi * std::cos(theta),
        radius * sin_phi * std::sin(theta),
        radius * std::cos(phi)
    );
}

}  // namespace temp2::geometry
