#include "geometry/shapes.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

namespace temp2::geometry {

// =============================================================================
// LineSegment
// =============================================================================

LineSegment::LineSegment() : start(), end() {}
LineSegment::LineSegment(const Point2D& start, const Point2D& end) : start(start), end(end) {}

double LineSegment::length() const { return start.distance_to(end); }
Point2D LineSegment::midpoint() const { return start.midpoint(end); }
Vector2D LineSegment::direction() const { return Vector2D(start, end).normalized(); }
Vector2D LineSegment::normal() const { return direction().perpendicular(); }

double LineSegment::distance_to_point(const Point2D& point) const {
    return point.distance_to(closest_point(point));
}

Point2D LineSegment::closest_point(const Point2D& point) const {
    Vector2D v(start, end);
    Vector2D w(start, point);
    double t = w.dot(v) / v.dot(v);
    t = std::max(0.0, std::min(1.0, t));
    return point_at(t);
}

std::optional<Point2D> LineSegment::intersection(const LineSegment& other) const {
    Vector2D r(start, end);
    Vector2D s(other.start, other.end);
    Vector2D qp(start, other.start);

    double rxs = r.cross(s);
    double qpxr = qp.cross(r);

    if (std::abs(rxs) < 1e-10) return std::nullopt;

    double t = qp.cross(s) / rxs;
    double u = qpxr / rxs;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        return point_at(t);
    }
    return std::nullopt;
}

bool LineSegment::intersects(const LineSegment& other) const {
    return intersection(other).has_value();
}

bool LineSegment::contains_point(const Point2D& point, double epsilon) const {
    return distance_to_point(point) < epsilon;
}

Point2D LineSegment::point_at(double t) const {
    return start.lerp(end, t);
}

std::string LineSegment::to_string() const {
    return start.to_string() + " -> " + end.to_string();
}

// =============================================================================
// Line
// =============================================================================

Line::Line() : point(), direction(1, 0) {}
Line::Line(const Point2D& p, const Vector2D& d) : point(p), direction(d.normalized()) {}

Line Line::from_points(const Point2D& p1, const Point2D& p2) {
    return Line(p1, Vector2D(p1, p2));
}
Line Line::from_segment(const LineSegment& seg) {
    return from_points(seg.start, seg.end);
}

double Line::distance_to_point(const Point2D& p) const {
    Vector2D v(point, p);
    return std::abs(v.cross(direction));
}

Point2D Line::closest_point(const Point2D& p) const {
    Vector2D v(point, p);
    return (Vector2D(point) + direction * v.dot(direction)).to_point();
}

std::optional<Point2D> Line::intersection(const Line& other) const {
    double cross = direction.cross(other.direction);
    if (std::abs(cross) < 1e-10) return std::nullopt;

    Vector2D diff(point, other.point);
    double t = diff.cross(other.direction) / cross;
    return point_at(t);
}

bool Line::is_parallel_to(const Line& other, double epsilon) const {
    return std::abs(direction.cross(other.direction)) < epsilon;
}

Point2D Line::point_at(double t) const {
    return (Vector2D(point) + direction * t).to_point();
}

// =============================================================================
// Circle
// =============================================================================

Circle::Circle() : center(), radius(1) {}
Circle::Circle(const Point2D& c, double r) : center(c), radius(r) {}

double Circle::area() const { return M_PI * radius * radius; }
double Circle::perimeter() const { return 2 * M_PI * radius; }
Point2D Circle::centroid() const { return center; }
bool Circle::contains(const Point2D& point) const {
    return center.distance_to(point) <= radius;
}
std::vector<Point2D> Circle::vertices() const { return {}; }

double Circle::diameter() const { return 2 * radius; }

bool Circle::intersects(const Circle& other) const {
    return center.distance_to(other.center) <= radius + other.radius;
}

bool Circle::intersects(const LineSegment& segment) const {
    return segment.distance_to_point(center) <= radius;
}

Point2D Circle::point_at_angle(double radians) const {
    return Point2D(center.x + radius * std::cos(radians),
                   center.y + radius * std::sin(radians));
}

std::vector<Point2D> Circle::sample_points(int count) const {
    std::vector<Point2D> points;
    for (int i = 0; i < count; ++i) {
        double angle = 2 * M_PI * i / count;
        points.push_back(point_at_angle(angle));
    }
    return points;
}

std::string Circle::to_string() const {
    std::ostringstream oss;
    oss << "Circle(" << center.to_string() << ", r=" << radius << ")";
    return oss.str();
}

// =============================================================================
// Rectangle
// =============================================================================

Rectangle::Rectangle() : position(), width(1), height(1) {}
Rectangle::Rectangle(const Point2D& pos, double w, double h)
    : position(pos), width(w), height(h) {}

Rectangle Rectangle::from_center(const Point2D& c, double w, double h) {
    return Rectangle(Point2D(c.x - w/2, c.y - h/2), w, h);
}
Rectangle Rectangle::from_corners(const Point2D& c1, const Point2D& c2) {
    double x = std::min(c1.x, c2.x);
    double y = std::min(c1.y, c2.y);
    double w = std::abs(c2.x - c1.x);
    double h = std::abs(c2.y - c1.y);
    return Rectangle(Point2D(x, y), w, h);
}

double Rectangle::area() const { return width * height; }
double Rectangle::perimeter() const { return 2 * (width + height); }
Point2D Rectangle::centroid() const { return center(); }
bool Rectangle::contains(const Point2D& p) const {
    return p.x >= min_x() && p.x <= max_x() && p.y >= min_y() && p.y <= max_y();
}
std::vector<Point2D> Rectangle::vertices() const {
    return {bottom_left(), bottom_right(), top_right(), top_left()};
}

Point2D Rectangle::top_left() const { return Point2D(position.x, position.y + height); }
Point2D Rectangle::top_right() const { return Point2D(position.x + width, position.y + height); }
Point2D Rectangle::bottom_left() const { return position; }
Point2D Rectangle::bottom_right() const { return Point2D(position.x + width, position.y); }
Point2D Rectangle::center() const { return Point2D(position.x + width/2, position.y + height/2); }

double Rectangle::min_x() const { return position.x; }
double Rectangle::max_x() const { return position.x + width; }
double Rectangle::min_y() const { return position.y; }
double Rectangle::max_y() const { return position.y + height; }

bool Rectangle::intersects(const Rectangle& other) const {
    return !(max_x() < other.min_x() || min_x() > other.max_x() ||
             max_y() < other.min_y() || min_y() > other.max_y());
}

std::string Rectangle::to_string() const {
    std::ostringstream oss;
    oss << "Rect(" << position.to_string() << ", " << width << "x" << height << ")";
    return oss.str();
}

// =============================================================================
// Triangle
// =============================================================================

Triangle::Triangle() : a(), b(1, 0), c(0.5, 1) {}
Triangle::Triangle(const Point2D& a, const Point2D& b, const Point2D& c)
    : a(a), b(b), c(c) {}

double Triangle::area() const {
    return 0.5 * std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
}
double Triangle::perimeter() const {
    return a.distance_to(b) + b.distance_to(c) + c.distance_to(a);
}
Point2D Triangle::centroid() const {
    return Point2D((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
}
bool Triangle::contains(const Point2D& p) const {
    auto sign = [](const Point2D& p1, const Point2D& p2, const Point2D& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };
    double d1 = sign(p, a, b);
    double d2 = sign(p, b, c);
    double d3 = sign(p, c, a);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}
std::vector<Point2D> Triangle::vertices() const { return {a, b, c}; }

std::vector<double> Triangle::sides() const {
    return {a.distance_to(b), b.distance_to(c), c.distance_to(a)};
}

std::vector<LineSegment> Triangle::edges() const {
    return {LineSegment(a, b), LineSegment(b, c), LineSegment(c, a)};
}

std::string Triangle::to_string() const {
    return "Triangle(" + a.to_string() + ", " + b.to_string() + ", " + c.to_string() + ")";
}

// =============================================================================
// Polygon
// =============================================================================

Polygon::Polygon() {}
Polygon::Polygon(const std::vector<Point2D>& pts) : points(pts) {}

double Polygon::area() const {
    if (points.size() < 3) return 0;
    double sum = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        sum += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    return std::abs(sum) / 2;
}

double Polygon::perimeter() const {
    if (points.size() < 2) return 0;
    double sum = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        sum += points[i].distance_to(points[j]);
    }
    return sum;
}

Point2D Polygon::centroid() const {
    if (points.empty()) return Point2D();
    double cx = 0, cy = 0;
    for (const auto& p : points) { cx += p.x; cy += p.y; }
    return Point2D(cx / points.size(), cy / points.size());
}

bool Polygon::contains(const Point2D& point) const {
    if (points.size() < 3) return false;
    int crossings = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        if ((points[i].y <= point.y && points[j].y > point.y) ||
            (points[j].y <= point.y && points[i].y > point.y)) {
            double x = points[i].x + (point.y - points[i].y) /
                      (points[j].y - points[i].y) * (points[j].x - points[i].x);
            if (point.x < x) ++crossings;
        }
    }
    return (crossings % 2) == 1;
}

std::vector<Point2D> Polygon::vertices() const { return points; }
size_t Polygon::vertex_count() const { return points.size(); }

bool Polygon::is_convex() const {
    if (points.size() < 3) return false;
    bool sign = false;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        size_t k = (i + 2) % points.size();
        Vector2D v1(points[i], points[j]);
        Vector2D v2(points[j], points[k]);
        double cross = v1.cross(v2);
        if (i == 0) sign = cross > 0;
        else if ((cross > 0) != sign) return false;
    }
    return true;
}

std::vector<LineSegment> Polygon::edges() const {
    std::vector<LineSegment> result;
    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        result.emplace_back(points[i], points[j]);
    }
    return result;
}

Polygon Polygon::regular(const Point2D& center, double radius, int sides) {
    std::vector<Point2D> pts;
    for (int i = 0; i < sides; ++i) {
        double angle = 2 * M_PI * i / sides - M_PI / 2;
        pts.push_back(Point2D(center.x + radius * std::cos(angle),
                              center.y + radius * std::sin(angle)));
    }
    return Polygon(pts);
}

std::string Polygon::to_string() const {
    std::ostringstream oss;
    oss << "Polygon(" << points.size() << " vertices)";
    return oss.str();
}

}  // namespace temp2::geometry
