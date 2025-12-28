#ifndef TEMP2_GEOMETRY_SHAPES_HPP
#define TEMP2_GEOMETRY_SHAPES_HPP

#include "point.hpp"
#include "vector2d.hpp"
#include <vector>
#include <optional>

namespace temp2::geometry {

/**
 * @brief Abstract base class for 2D shapes
 */
class Shape2D {
public:
    virtual ~Shape2D() = default;

    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual Point2D centroid() const = 0;
    virtual bool contains(const Point2D& point) const = 0;
    virtual std::vector<Point2D> vertices() const = 0;
};

/**
 * @brief Line segment in 2D
 */
class LineSegment {
public:
    Point2D start;
    Point2D end;

    LineSegment();
    LineSegment(const Point2D& start, const Point2D& end);

    double length() const;
    Point2D midpoint() const;
    Vector2D direction() const;
    Vector2D normal() const;

    double distance_to_point(const Point2D& point) const;
    Point2D closest_point(const Point2D& point) const;
    std::optional<Point2D> intersection(const LineSegment& other) const;
    bool intersects(const LineSegment& other) const;
    bool contains_point(const Point2D& point, double epsilon = 1e-9) const;

    Point2D point_at(double t) const;  // t in [0, 1]

    std::string to_string() const;
};

/**
 * @brief Line (infinite, defined by point and direction)
 */
class Line {
public:
    Point2D point;
    Vector2D direction;

    Line();
    Line(const Point2D& point, const Vector2D& direction);
    static Line from_points(const Point2D& p1, const Point2D& p2);
    static Line from_segment(const LineSegment& segment);

    double distance_to_point(const Point2D& p) const;
    Point2D closest_point(const Point2D& p) const;
    std::optional<Point2D> intersection(const Line& other) const;
    bool is_parallel_to(const Line& other, double epsilon = 1e-9) const;

    Point2D point_at(double t) const;
};

/**
 * @brief Circle shape
 */
class Circle : public Shape2D {
public:
    Point2D center;
    double radius;

    Circle();
    Circle(const Point2D& center, double radius);

    // Shape2D interface
    double area() const override;
    double perimeter() const override;
    Point2D centroid() const override;
    bool contains(const Point2D& point) const override;
    std::vector<Point2D> vertices() const override;

    // Circle-specific
    double diameter() const;
    bool intersects(const Circle& other) const;
    bool intersects(const LineSegment& segment) const;
    std::vector<Point2D> intersection_points(const Line& line) const;
    std::vector<Point2D> intersection_points(const Circle& other) const;

    Point2D point_at_angle(double radians) const;
    std::vector<Point2D> sample_points(int count) const;

    std::string to_string() const;
};

/**
 * @brief Axis-aligned rectangle
 */
class Rectangle : public Shape2D {
public:
    Point2D position;  // Bottom-left corner
    double width;
    double height;

    Rectangle();
    Rectangle(const Point2D& position, double width, double height);
    static Rectangle from_center(const Point2D& center, double width, double height);
    static Rectangle from_corners(const Point2D& corner1, const Point2D& corner2);

    // Shape2D interface
    double area() const override;
    double perimeter() const override;
    Point2D centroid() const override;
    bool contains(const Point2D& point) const override;
    std::vector<Point2D> vertices() const override;

    // Rectangle-specific
    Point2D top_left() const;
    Point2D top_right() const;
    Point2D bottom_left() const;
    Point2D bottom_right() const;
    Point2D center() const;

    double min_x() const;
    double max_x() const;
    double min_y() const;
    double max_y() const;

    bool intersects(const Rectangle& other) const;
    std::optional<Rectangle> intersection(const Rectangle& other) const;
    Rectangle bounding_union(const Rectangle& other) const;

    std::vector<LineSegment> edges() const;

    std::string to_string() const;
};

/**
 * @brief Triangle shape
 */
class Triangle : public Shape2D {
public:
    Point2D a;
    Point2D b;
    Point2D c;

    Triangle();
    Triangle(const Point2D& a, const Point2D& b, const Point2D& c);

    // Shape2D interface
    double area() const override;
    double perimeter() const override;
    Point2D centroid() const override;
    bool contains(const Point2D& point) const override;
    std::vector<Point2D> vertices() const override;

    // Triangle-specific
    Point2D incenter() const;
    Point2D circumcenter() const;
    Point2D orthocenter() const;

    double inradius() const;
    double circumradius() const;

    std::vector<double> sides() const;
    std::vector<double> angles() const;

    bool is_equilateral(double epsilon = 1e-9) const;
    bool is_isosceles(double epsilon = 1e-9) const;
    bool is_right(double epsilon = 1e-9) const;
    bool is_acute() const;
    bool is_obtuse() const;

    Point2D barycentric_to_cartesian(double u, double v, double w) const;
    std::tuple<double, double, double> cartesian_to_barycentric(const Point2D& point) const;

    std::vector<LineSegment> edges() const;
    Circle circumscribed_circle() const;
    Circle inscribed_circle() const;

    std::string to_string() const;
};

/**
 * @brief General polygon
 */
class Polygon : public Shape2D {
public:
    std::vector<Point2D> points;

    Polygon();
    explicit Polygon(const std::vector<Point2D>& points);

    // Shape2D interface
    double area() const override;
    double perimeter() const override;
    Point2D centroid() const override;
    bool contains(const Point2D& point) const override;
    std::vector<Point2D> vertices() const override;

    // Polygon-specific
    size_t vertex_count() const;
    bool is_convex() const;
    bool is_simple() const;  // No self-intersections
    bool is_clockwise() const;

    void reverse_winding();
    Polygon convex_hull() const;
    Rectangle bounding_box() const;

    std::vector<LineSegment> edges() const;

    static Polygon regular(const Point2D& center, double radius, int sides);

    std::string to_string() const;
};

}  // namespace temp2::geometry

#endif  // TEMP2_GEOMETRY_SHAPES_HPP
