#ifndef TEMP2_GEOMETRY_TRANSFORMS_HPP
#define TEMP2_GEOMETRY_TRANSFORMS_HPP

#include "point.hpp"
#include "vector2d.hpp"
#include <array>

namespace temp2::geometry {

/**
 * @brief 2D transformation matrix (3x3 for homogeneous coordinates)
 */
class Transform2D {
public:
    Transform2D();

    // Factory methods
    static Transform2D identity();
    static Transform2D translation(double tx, double ty);
    static Transform2D translation(const Vector2D& offset);
    static Transform2D rotation(double radians);
    static Transform2D rotation(double radians, const Point2D& center);
    static Transform2D scaling(double sx, double sy);
    static Transform2D scaling(double s);
    static Transform2D scaling(double sx, double sy, const Point2D& center);
    static Transform2D shear_x(double factor);
    static Transform2D shear_y(double factor);
    static Transform2D reflection_x();
    static Transform2D reflection_y();
    static Transform2D reflection(const Point2D& line_point, const Vector2D& line_dir);

    // Combine transforms
    Transform2D operator*(const Transform2D& other) const;
    Transform2D& operator*=(const Transform2D& other);

    // Apply transform
    Point2D apply(const Point2D& point) const;
    Vector2D apply(const Vector2D& vec) const;
    std::vector<Point2D> apply(const std::vector<Point2D>& points) const;

    // Inverse
    Transform2D inverse() const;
    bool is_invertible() const;

    // Properties
    double determinant() const;
    bool is_identity() const;

    // Direct matrix access
    double& at(int row, int col);
    double at(int row, int col) const;

    std::string to_string() const;

private:
    std::array<std::array<double, 3>, 3> matrix_;
};

/**
 * @brief 3D transformation matrix (4x4 for homogeneous coordinates)
 */
class Transform3D {
public:
    Transform3D();

    // Factory methods
    static Transform3D identity();
    static Transform3D translation(double tx, double ty, double tz);
    static Transform3D translation(const Vector3D& offset);
    static Transform3D rotation_x(double radians);
    static Transform3D rotation_y(double radians);
    static Transform3D rotation_z(double radians);
    static Transform3D rotation_axis(const Vector3D& axis, double radians);
    static Transform3D scaling(double sx, double sy, double sz);
    static Transform3D scaling(double s);
    static Transform3D look_at(const Point3D& eye, const Point3D& target, const Vector3D& up);
    static Transform3D perspective(double fov, double aspect, double near, double far);
    static Transform3D orthographic(double left, double right, double bottom, double top,
                                     double near, double far);

    // Combine transforms
    Transform3D operator*(const Transform3D& other) const;
    Transform3D& operator*=(const Transform3D& other);

    // Apply transform
    Point3D apply(const Point3D& point) const;
    Vector3D apply(const Vector3D& vec) const;
    std::vector<Point3D> apply(const std::vector<Point3D>& points) const;

    // Inverse
    Transform3D inverse() const;
    bool is_invertible() const;

    // Properties
    double determinant() const;
    bool is_identity() const;

    // Direct matrix access
    double& at(int row, int col);
    double at(int row, int col) const;

    std::string to_string() const;

private:
    std::array<std::array<double, 4>, 4> matrix_;
};

/**
 * @brief Quaternion for 3D rotations
 */
class Quaternion {
public:
    double w, x, y, z;

    Quaternion();
    Quaternion(double w, double x, double y, double z);

    // Factory methods
    static Quaternion identity();
    static Quaternion from_axis_angle(const Vector3D& axis, double radians);
    static Quaternion from_euler(double pitch, double yaw, double roll);
    static Quaternion from_rotation_matrix(const Transform3D& matrix);
    static Quaternion look_rotation(const Vector3D& forward, const Vector3D& up = Vector3D::up());

    // Arithmetic
    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(double scalar) const;
    Quaternion operator/(double scalar) const;

    // Properties
    double magnitude() const;
    double magnitude_squared() const;
    Quaternion normalized() const;
    Quaternion conjugate() const;
    Quaternion inverse() const;

    // Conversion
    Transform3D to_matrix() const;
    Vector3D to_euler() const;
    void to_axis_angle(Vector3D& axis, double& angle) const;

    // Apply rotation
    Vector3D rotate(const Vector3D& v) const;
    Point3D rotate(const Point3D& p) const;

    // Interpolation
    static Quaternion lerp(const Quaternion& a, const Quaternion& b, double t);
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, double t);
    static Quaternion squad(const Quaternion& q0, const Quaternion& q1,
                             const Quaternion& s0, const Quaternion& s1, double t);

    // Utility
    double dot(const Quaternion& other) const;
    double angle_between(const Quaternion& other) const;
    bool near_equal(const Quaternion& other, double epsilon = 1e-9) const;

    std::string to_string() const;
};

}  // namespace temp2::geometry

#endif  // TEMP2_GEOMETRY_TRANSFORMS_HPP
