#ifndef TEMP2_GEOMETRY_POINT_HPP
#define TEMP2_GEOMETRY_POINT_HPP

#include <cmath>
#include <string>

namespace temp2::geometry {

/**
 * @brief 2D Point class
 */
class Point2D {
public:
    double x;
    double y;

    Point2D();
    Point2D(double x, double y);

    // Arithmetic
    Point2D operator+(const Point2D& other) const;
    Point2D operator-(const Point2D& other) const;
    Point2D operator*(double scalar) const;
    Point2D operator/(double scalar) const;

    Point2D& operator+=(const Point2D& other);
    Point2D& operator-=(const Point2D& other);
    Point2D& operator*=(double scalar);
    Point2D& operator/=(double scalar);

    // Unary
    Point2D operator-() const;

    // Comparison
    bool operator==(const Point2D& other) const;
    bool operator!=(const Point2D& other) const;

    // Distance and magnitude
    double distance_to(const Point2D& other) const;
    double distance_to_origin() const;
    double magnitude() const;
    double magnitude_squared() const;

    // Operations
    Point2D normalized() const;
    Point2D rotated(double radians) const;
    Point2D rotated_around(const Point2D& center, double radians) const;
    Point2D lerp(const Point2D& target, double t) const;
    Point2D midpoint(const Point2D& other) const;
    Point2D reflected(const Point2D& normal) const;

    // Dot and cross product
    double dot(const Point2D& other) const;
    double cross(const Point2D& other) const;

    // Angle
    double angle() const;
    double angle_to(const Point2D& other) const;

    // Utility
    std::string to_string() const;
    bool is_zero() const;
    bool near_equal(const Point2D& other, double epsilon = 1e-9) const;

    // Static factory methods
    static Point2D from_polar(double radius, double angle);
    static Point2D zero();
    static Point2D unit_x();
    static Point2D unit_y();
};

/**
 * @brief 3D Point class
 */
class Point3D {
public:
    double x;
    double y;
    double z;

    Point3D();
    Point3D(double x, double y, double z);

    // Arithmetic
    Point3D operator+(const Point3D& other) const;
    Point3D operator-(const Point3D& other) const;
    Point3D operator*(double scalar) const;
    Point3D operator/(double scalar) const;

    Point3D& operator+=(const Point3D& other);
    Point3D& operator-=(const Point3D& other);
    Point3D& operator*=(double scalar);
    Point3D& operator/=(double scalar);

    // Unary
    Point3D operator-() const;

    // Comparison
    bool operator==(const Point3D& other) const;
    bool operator!=(const Point3D& other) const;

    // Distance and magnitude
    double distance_to(const Point3D& other) const;
    double distance_to_origin() const;
    double magnitude() const;
    double magnitude_squared() const;

    // Operations
    Point3D normalized() const;
    Point3D lerp(const Point3D& target, double t) const;
    Point3D midpoint(const Point3D& other) const;

    // Products
    double dot(const Point3D& other) const;
    Point3D cross(const Point3D& other) const;

    // Projections
    Point3D project_onto(const Point3D& onto) const;
    Point3D project_onto_plane(const Point3D& normal) const;

    // Utility
    std::string to_string() const;
    bool is_zero() const;
    bool near_equal(const Point3D& other, double epsilon = 1e-9) const;

    // Static factory methods
    static Point3D zero();
    static Point3D unit_x();
    static Point3D unit_y();
    static Point3D unit_z();
    static Point3D from_spherical(double radius, double theta, double phi);
};

}  // namespace temp2::geometry

#endif  // TEMP2_GEOMETRY_POINT_HPP
