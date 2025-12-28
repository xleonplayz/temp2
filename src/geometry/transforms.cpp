#include "geometry/transforms.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

namespace temp2::geometry {

// =============================================================================
// Transform2D
// =============================================================================

Transform2D::Transform2D() : matrix_{} {
    for (int i = 0; i < 3; ++i) matrix_[i][i] = 1.0;
}

Transform2D Transform2D::identity() { return Transform2D(); }

Transform2D Transform2D::translation(double tx, double ty) {
    Transform2D t;
    t.matrix_[0][2] = tx;
    t.matrix_[1][2] = ty;
    return t;
}
Transform2D Transform2D::translation(const Vector2D& offset) {
    return translation(offset.x, offset.y);
}

Transform2D Transform2D::rotation(double radians) {
    Transform2D t;
    double c = std::cos(radians), s = std::sin(radians);
    t.matrix_[0][0] = c; t.matrix_[0][1] = -s;
    t.matrix_[1][0] = s; t.matrix_[1][1] = c;
    return t;
}

Transform2D Transform2D::rotation(double radians, const Point2D& center) {
    return translation(center.x, center.y) * rotation(radians) * translation(-center.x, -center.y);
}

Transform2D Transform2D::scaling(double sx, double sy) {
    Transform2D t;
    t.matrix_[0][0] = sx;
    t.matrix_[1][1] = sy;
    return t;
}

Transform2D Transform2D::scaling(double s) { return scaling(s, s); }

Transform2D Transform2D::scaling(double sx, double sy, const Point2D& center) {
    return translation(center.x, center.y) * scaling(sx, sy) * translation(-center.x, -center.y);
}

Transform2D Transform2D::shear_x(double factor) {
    Transform2D t;
    t.matrix_[0][1] = factor;
    return t;
}

Transform2D Transform2D::shear_y(double factor) {
    Transform2D t;
    t.matrix_[1][0] = factor;
    return t;
}

Transform2D Transform2D::reflection_x() { return scaling(1, -1); }
Transform2D Transform2D::reflection_y() { return scaling(-1, 1); }

Transform2D Transform2D::operator*(const Transform2D& other) const {
    Transform2D result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.matrix_[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
            }
        }
    }
    return result;
}

Transform2D& Transform2D::operator*=(const Transform2D& other) {
    *this = *this * other;
    return *this;
}

Point2D Transform2D::apply(const Point2D& point) const {
    return Point2D(
        matrix_[0][0] * point.x + matrix_[0][1] * point.y + matrix_[0][2],
        matrix_[1][0] * point.x + matrix_[1][1] * point.y + matrix_[1][2]
    );
}

Vector2D Transform2D::apply(const Vector2D& vec) const {
    return Vector2D(
        matrix_[0][0] * vec.x + matrix_[0][1] * vec.y,
        matrix_[1][0] * vec.x + matrix_[1][1] * vec.y
    );
}

std::vector<Point2D> Transform2D::apply(const std::vector<Point2D>& points) const {
    std::vector<Point2D> result;
    result.reserve(points.size());
    for (const auto& p : points) result.push_back(apply(p));
    return result;
}

double Transform2D::determinant() const {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
}

bool Transform2D::is_invertible() const { return std::abs(determinant()) > 1e-10; }

Transform2D Transform2D::inverse() const {
    double det = determinant();
    Transform2D inv;
    inv.matrix_[0][0] = matrix_[1][1] / det;
    inv.matrix_[0][1] = -matrix_[0][1] / det;
    inv.matrix_[1][0] = -matrix_[1][0] / det;
    inv.matrix_[1][1] = matrix_[0][0] / det;
    inv.matrix_[0][2] = -(inv.matrix_[0][0] * matrix_[0][2] + inv.matrix_[0][1] * matrix_[1][2]);
    inv.matrix_[1][2] = -(inv.matrix_[1][0] * matrix_[0][2] + inv.matrix_[1][1] * matrix_[1][2]);
    return inv;
}

double& Transform2D::at(int row, int col) { return matrix_[row][col]; }
double Transform2D::at(int row, int col) const { return matrix_[row][col]; }

std::string Transform2D::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    for (int i = 0; i < 3; ++i) {
        oss << "[";
        for (int j = 0; j < 3; ++j) {
            if (j > 0) oss << ", ";
            oss << matrix_[i][j];
        }
        oss << "]\n";
    }
    return oss.str();
}

// =============================================================================
// Transform3D
// =============================================================================

Transform3D::Transform3D() : matrix_{} {
    for (int i = 0; i < 4; ++i) matrix_[i][i] = 1.0;
}

Transform3D Transform3D::identity() { return Transform3D(); }

Transform3D Transform3D::translation(double tx, double ty, double tz) {
    Transform3D t;
    t.matrix_[0][3] = tx; t.matrix_[1][3] = ty; t.matrix_[2][3] = tz;
    return t;
}
Transform3D Transform3D::translation(const Vector3D& offset) {
    return translation(offset.x, offset.y, offset.z);
}

Transform3D Transform3D::rotation_x(double radians) {
    Transform3D t;
    double c = std::cos(radians), s = std::sin(radians);
    t.matrix_[1][1] = c; t.matrix_[1][2] = -s;
    t.matrix_[2][1] = s; t.matrix_[2][2] = c;
    return t;
}

Transform3D Transform3D::rotation_y(double radians) {
    Transform3D t;
    double c = std::cos(radians), s = std::sin(radians);
    t.matrix_[0][0] = c; t.matrix_[0][2] = s;
    t.matrix_[2][0] = -s; t.matrix_[2][2] = c;
    return t;
}

Transform3D Transform3D::rotation_z(double radians) {
    Transform3D t;
    double c = std::cos(radians), s = std::sin(radians);
    t.matrix_[0][0] = c; t.matrix_[0][1] = -s;
    t.matrix_[1][0] = s; t.matrix_[1][1] = c;
    return t;
}

Transform3D Transform3D::scaling(double sx, double sy, double sz) {
    Transform3D t;
    t.matrix_[0][0] = sx; t.matrix_[1][1] = sy; t.matrix_[2][2] = sz;
    return t;
}

Transform3D Transform3D::scaling(double s) { return scaling(s, s, s); }

Transform3D Transform3D::operator*(const Transform3D& other) const {
    Transform3D result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.matrix_[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
            }
        }
    }
    return result;
}

Point3D Transform3D::apply(const Point3D& point) const {
    double w = matrix_[3][0] * point.x + matrix_[3][1] * point.y + matrix_[3][2] * point.z + matrix_[3][3];
    return Point3D(
        (matrix_[0][0] * point.x + matrix_[0][1] * point.y + matrix_[0][2] * point.z + matrix_[0][3]) / w,
        (matrix_[1][0] * point.x + matrix_[1][1] * point.y + matrix_[1][2] * point.z + matrix_[1][3]) / w,
        (matrix_[2][0] * point.x + matrix_[2][1] * point.y + matrix_[2][2] * point.z + matrix_[2][3]) / w
    );
}

Vector3D Transform3D::apply(const Vector3D& vec) const {
    return Vector3D(
        matrix_[0][0] * vec.x + matrix_[0][1] * vec.y + matrix_[0][2] * vec.z,
        matrix_[1][0] * vec.x + matrix_[1][1] * vec.y + matrix_[1][2] * vec.z,
        matrix_[2][0] * vec.x + matrix_[2][1] * vec.y + matrix_[2][2] * vec.z
    );
}

double& Transform3D::at(int row, int col) { return matrix_[row][col]; }
double Transform3D::at(int row, int col) const { return matrix_[row][col]; }

// =============================================================================
// Quaternion
// =============================================================================

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}
Quaternion::Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::identity() { return Quaternion(1, 0, 0, 0); }

Quaternion Quaternion::from_axis_angle(const Vector3D& axis, double radians) {
    Vector3D n = axis.normalized();
    double half = radians / 2;
    double s = std::sin(half);
    return Quaternion(std::cos(half), n.x * s, n.y * s, n.z * s);
}

Quaternion Quaternion::operator+(const Quaternion& o) const { return Quaternion(w + o.w, x + o.x, y + o.y, z + o.z); }
Quaternion Quaternion::operator-(const Quaternion& o) const { return Quaternion(w - o.w, x - o.x, y - o.y, z - o.z); }
Quaternion Quaternion::operator*(double s) const { return Quaternion(w * s, x * s, y * s, z * s); }
Quaternion Quaternion::operator/(double s) const { return Quaternion(w / s, x / s, y / s, z / s); }

Quaternion Quaternion::operator*(const Quaternion& o) const {
    return Quaternion(
        w * o.w - x * o.x - y * o.y - z * o.z,
        w * o.x + x * o.w + y * o.z - z * o.y,
        w * o.y - x * o.z + y * o.w + z * o.x,
        w * o.z + x * o.y - y * o.x + z * o.w
    );
}

double Quaternion::magnitude() const { return std::sqrt(w * w + x * x + y * y + z * z); }
double Quaternion::magnitude_squared() const { return w * w + x * x + y * y + z * z; }
Quaternion Quaternion::normalized() const { double m = magnitude(); return Quaternion(w / m, x / m, y / m, z / m); }
Quaternion Quaternion::conjugate() const { return Quaternion(w, -x, -y, -z); }
Quaternion Quaternion::inverse() const { return conjugate() / magnitude_squared(); }

Vector3D Quaternion::rotate(const Vector3D& v) const {
    Quaternion p(0, v.x, v.y, v.z);
    Quaternion result = *this * p * conjugate();
    return Vector3D(result.x, result.y, result.z);
}

Point3D Quaternion::rotate(const Point3D& p) const {
    Vector3D v = rotate(Vector3D(p));
    return v.to_point();
}

double Quaternion::dot(const Quaternion& o) const { return w * o.w + x * o.x + y * o.y + z * o.z; }

Quaternion Quaternion::lerp(const Quaternion& a, const Quaternion& b, double t) {
    return (a * (1 - t) + b * t).normalized();
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, double t) {
    double d = a.dot(b);
    Quaternion b2 = d < 0 ? b * -1 : b;
    d = std::abs(d);
    if (d > 0.9995) return lerp(a, b2, t);
    double theta = std::acos(d);
    double s = std::sin(theta);
    return a * (std::sin((1 - t) * theta) / s) + b2 * (std::sin(t * theta) / s);
}

std::string Quaternion::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "Quaternion(" << w << ", " << x << ", " << y << ", " << z << ")";
    return oss.str();
}

}  // namespace temp2::geometry
