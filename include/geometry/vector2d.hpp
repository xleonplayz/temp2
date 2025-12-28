#ifndef TEMP2_GEOMETRY_VECTOR2D_HPP
#define TEMP2_GEOMETRY_VECTOR2D_HPP

#include "point.hpp"

namespace temp2::geometry {

/**
 * @brief 2D Vector class (direction and magnitude)
 */
class Vector2D {
public:
    double x;
    double y;

    Vector2D();
    Vector2D(double x, double y);
    explicit Vector2D(const Point2D& point);
    Vector2D(const Point2D& from, const Point2D& to);

    // Arithmetic
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    Vector2D operator/(double scalar) const;

    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(double scalar);
    Vector2D& operator/=(double scalar);

    Vector2D operator-() const;

    // Comparison
    bool operator==(const Vector2D& other) const;
    bool operator!=(const Vector2D& other) const;

    // Magnitude and normalization
    double length() const;
    double length_squared() const;
    Vector2D normalized() const;
    void normalize();

    // Products
    double dot(const Vector2D& other) const;
    double cross(const Vector2D& other) const;

    // Angle operations
    double angle() const;
    double angle_between(const Vector2D& other) const;
    bool is_parallel_to(const Vector2D& other, double epsilon = 1e-9) const;
    bool is_perpendicular_to(const Vector2D& other, double epsilon = 1e-9) const;

    // Transformations
    Vector2D rotated(double radians) const;
    Vector2D perpendicular() const;
    Vector2D reflected(const Vector2D& normal) const;
    Vector2D projected_onto(const Vector2D& other) const;
    Vector2D rejected_from(const Vector2D& other) const;

    // Linear interpolation
    Vector2D lerp(const Vector2D& target, double t) const;
    Vector2D slerp(const Vector2D& target, double t) const;

    // Utility
    std::string to_string() const;
    bool is_zero() const;
    bool near_equal(const Vector2D& other, double epsilon = 1e-9) const;
    Point2D to_point() const;

    // Static factory methods
    static Vector2D from_angle(double radians);
    static Vector2D zero();
    static Vector2D up();
    static Vector2D down();
    static Vector2D left();
    static Vector2D right();
};

/**
 * @brief 3D Vector class
 */
class Vector3D {
public:
    double x;
    double y;
    double z;

    Vector3D();
    Vector3D(double x, double y, double z);
    explicit Vector3D(const Point3D& point);
    Vector3D(const Point3D& from, const Point3D& to);

    // Arithmetic
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;

    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(double scalar);
    Vector3D& operator/=(double scalar);

    Vector3D operator-() const;

    // Comparison
    bool operator==(const Vector3D& other) const;
    bool operator!=(const Vector3D& other) const;

    // Magnitude and normalization
    double length() const;
    double length_squared() const;
    Vector3D normalized() const;
    void normalize();

    // Products
    double dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;
    double scalar_triple(const Vector3D& b, const Vector3D& c) const;

    // Angle operations
    double angle_between(const Vector3D& other) const;
    bool is_parallel_to(const Vector3D& other, double epsilon = 1e-9) const;
    bool is_perpendicular_to(const Vector3D& other, double epsilon = 1e-9) const;

    // Transformations
    Vector3D rotated_around_x(double radians) const;
    Vector3D rotated_around_y(double radians) const;
    Vector3D rotated_around_z(double radians) const;
    Vector3D rotated_around_axis(const Vector3D& axis, double radians) const;
    Vector3D projected_onto(const Vector3D& other) const;
    Vector3D projected_onto_plane(const Vector3D& normal) const;
    Vector3D reflected(const Vector3D& normal) const;

    // Linear interpolation
    Vector3D lerp(const Vector3D& target, double t) const;

    // Utility
    std::string to_string() const;
    bool is_zero() const;
    bool near_equal(const Vector3D& other, double epsilon = 1e-9) const;
    Point3D to_point() const;

    // Static factory methods
    static Vector3D zero();
    static Vector3D unit_x();
    static Vector3D unit_y();
    static Vector3D unit_z();
    static Vector3D up();
    static Vector3D forward();
    static Vector3D right();
};

}  // namespace temp2::geometry

#endif  // TEMP2_GEOMETRY_VECTOR2D_HPP
