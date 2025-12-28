#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Math library
#include "math/basic_math.hpp"
#include "math/advanced_math.hpp"
#include "math/statistics.hpp"

// String library
#include "strings/string_utils.hpp"
#include "strings/formatter.hpp"
#include "strings/parser.hpp"

// Container library
#include "containers/linked_list.hpp"
#include "containers/stack.hpp"
#include "containers/queue.hpp"
#include "containers/binary_tree.hpp"

// Geometry library
#include "geometry/point.hpp"
#include "geometry/vector2d.hpp"
#include "geometry/shapes.hpp"
#include "geometry/transforms.hpp"

// Algorithms library
#include "algorithms/sorting.hpp"
#include "algorithms/searching.hpp"
#include "algorithms/graph.hpp"

// Events library
#include "events/event.hpp"
#include "events/dispatcher.hpp"
#include "events/handler.hpp"

void demo_math() {
    std::cout << "\n=== Math Library Demo ===\n";

    // Basic math (static methods)
    std::cout << "abs(-5.5) = " << temp2::math::BasicMath::abs(-5.5) << "\n";
    std::cout << "max(3, 7) = " << temp2::math::BasicMath::max(3.0, 7.0) << "\n";
    std::cout << "clamp(15, 0, 10) = " << temp2::math::BasicMath::clamp(15.0, 0.0, 10.0) << "\n";

    // Trigonometry
    std::cout << "sin(PI/2) = " << temp2::math::Trigonometry::sin(temp2::math::Trigonometry::PI / 2) << "\n";
    std::cout << "radians_to_degrees(PI) = " << temp2::math::Trigonometry::radians_to_degrees(temp2::math::Trigonometry::PI) << "\n";

    // Statistics
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    std::cout << "mean = " << temp2::math::Statistics::mean(data) << "\n";
    std::cout << "variance = " << temp2::math::Statistics::variance(data) << "\n";
    std::cout << "stddev = " << temp2::math::Statistics::standard_deviation(data) << "\n";

    // Matrix
    temp2::math::Matrix2x2 mat(1, 2, 3, 4);
    std::cout << "Matrix determinant = " << mat.determinant() << "\n";
}

void demo_strings() {
    std::cout << "\n=== String Library Demo ===\n";

    // String utils
    temp2::strings::StringUtils utils;
    std::string text = "  Hello World  ";
    std::cout << "Original: '" << text << "'\n";
    std::cout << "Trimmed: '" << utils.trim(text) << "'\n";
    std::cout << "Upper: '" << utils.to_upper(text) << "'\n";

    // Split
    std::string csv = "apple,banana,cherry";
    auto parts = utils.split(csv, ',');
    std::cout << "Split '" << csv << "': ";
    for (const auto& p : parts) std::cout << "[" << p << "] ";
    std::cout << "\n";

    // Formatter
    std::cout << "Format fixed 1234567.89 = " << temp2::strings::NumberFormatter::format_fixed(1234567.89, 2) << "\n";
    std::cout << "Currency: " << temp2::strings::NumberFormatter::format_currency(99.99, "$") << "\n";

    // Parser
    temp2::strings::NumberParser parser;
    auto parsed = parser.parse_int("42");
    if (parsed) std::cout << "Parsed '42' = " << *parsed << "\n";
}

void demo_containers() {
    std::cout << "\n=== Container Library Demo ===\n";

    // Linked list
    temp2::containers::DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_front(0);
    std::cout << "DoublyLinkedList: ";
    for (size_t i = 0; i < list.size(); ++i) {
        std::cout << list.at(i) << " ";
    }
    std::cout << "\n";

    // Stack
    temp2::containers::ArrayStack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout << "Stack top: " << stack.top() << "\n";
    stack.pop();
    std::cout << "After pop, top: " << stack.top() << "\n";

    // Queue
    temp2::containers::CircularQueue<int> queue(5);
    queue.enqueue(100);
    queue.enqueue(200);
    queue.enqueue(300);
    std::cout << "Queue front: " << queue.front() << "\n";
    queue.dequeue();
    std::cout << "After dequeue, front: " << queue.front() << "\n";

    // Binary search tree
    temp2::containers::BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(1);
    bst.insert(9);
    std::cout << "BST contains 7: " << (bst.contains(7) ? "yes" : "no") << "\n";
    std::cout << "BST inorder: ";
    auto inorder = bst.inorder();
    for (int val : inorder) std::cout << val << " ";
    std::cout << "\n";
}

void demo_geometry() {
    std::cout << "\n=== Geometry Library Demo ===\n";

    // Points
    temp2::geometry::Point2D p1(0, 0);
    temp2::geometry::Point2D p2(3, 4);
    std::cout << "Distance from (0,0) to (3,4) = " << p1.distance_to(p2) << "\n";

    // Vectors
    temp2::geometry::Vector2D v1(1, 0);
    temp2::geometry::Vector2D v2(0, 1);
    std::cout << "Dot product of (1,0) and (0,1) = " << v1.dot(v2) << "\n";
    std::cout << "(1,0) length = " << v1.length() << "\n";

    // Shapes
    temp2::geometry::Circle circle(temp2::geometry::Point2D(0, 0), 5);
    std::cout << "Circle area = " << circle.area() << "\n";
    std::cout << "Circle perimeter = " << circle.perimeter() << "\n";

    temp2::geometry::Rectangle rect(
        temp2::geometry::Point2D(0, 0),
        10, 5
    );
    std::cout << "Rectangle area = " << rect.area() << "\n";

    // Transforms
    auto transform = temp2::geometry::Transform2D::translation(10, 20);
    auto transformed = transform.apply(temp2::geometry::Point2D(0, 0));
    std::cout << "Translated (0,0) by (10,20) = (" << transformed.x << ", " << transformed.y << ")\n";
}

void demo_algorithms() {
    std::cout << "\n=== Algorithms Library Demo ===\n";

    // Sorting
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    std::cout << "Original: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    temp2::algorithms::Sorting::quick_sort(arr);
    std::cout << "After quicksort: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // Searching
    auto found = temp2::algorithms::Searching::binary_search(arr, 25);
    if (found) {
        std::cout << "Found 25 at index " << *found << "\n";
    }

    // Graph
    temp2::algorithms::Graph graph(6);
    graph.add_edge(0, 1, 4);
    graph.add_edge(0, 2, 2);
    graph.add_edge(1, 2, 1);
    graph.add_edge(1, 3, 5);
    graph.add_edge(2, 3, 8);
    graph.add_edge(2, 4, 10);
    graph.add_edge(3, 4, 2);
    graph.add_edge(3, 5, 6);
    graph.add_edge(4, 5, 3);

    std::cout << "BFS from 0: ";
    auto bfs = graph.bfs(0);
    for (int v : bfs) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "DFS from 0: ";
    auto dfs = graph.dfs(0);
    for (int v : dfs) std::cout << v << " ";
    std::cout << "\n";

    auto distances = graph.dijkstra(0);
    std::cout << "Dijkstra distances from 0: ";
    for (size_t i = 0; i < distances.size(); ++i) {
        std::cout << i << ":" << distances[i] << " ";
    }
    std::cout << "\n";
}

void demo_events() {
    std::cout << "\n=== Events Library Demo ===\n";

    temp2::events::EventDispatcher dispatcher;

    // Subscribe to mouse events
    auto mouseId = dispatcher.subscribe("mouse", [](temp2::events::Event& e) {
        std::cout << "Mouse event received!\n";
    });
    (void)mouseId;  // Suppress unused variable warning

    // Subscribe to keyboard events with priority
    dispatcher.subscribe("keyboard", [](temp2::events::Event& e) {
        std::cout << "Keyboard event (priority 10)\n";
    }, 10);

    dispatcher.subscribe("keyboard", [](temp2::events::Event& e) {
        std::cout << "Keyboard event (priority 5)\n";
    }, 5);

    // Create and dispatch events
    temp2::events::MouseEvent mouseEvent(
        temp2::events::MouseEvent::Action::Press,
        100.0, 200.0,
        temp2::events::MouseEvent::Button::Left
    );
    dispatcher.dispatch(mouseEvent);

    temp2::events::KeyboardEvent keyEvent(
        temp2::events::KeyboardEvent::Action::Press,
        65, 0
    );
    dispatcher.dispatch(keyEvent);

    // Chained handler
    temp2::events::ChainedHandler chain;
    chain.then([](temp2::events::Event& e) {
        std::cout << "Chain step 1\n";
    }).then([](temp2::events::Event& e) {
        std::cout << "Chain step 2\n";
    });

    temp2::events::CustomEvent customEvent("test");
    chain(customEvent);

    std::cout << "Handler count for 'mouse': " << dispatcher.handler_count("mouse") << "\n";
    std::cout << "Handler count for 'keyboard': " << dispatcher.handler_count("keyboard") << "\n";
}

int main() {
    std::cout << "=== temp2 C++ Test Repository ===\n";
    std::cout << "Demonstrating various C++ patterns for brickc analysis\n";

    demo_math();
    demo_strings();
    demo_containers();
    demo_geometry();
    demo_algorithms();
    demo_events();

    std::cout << "\n=== All demos complete ===\n";
    return 0;
}
