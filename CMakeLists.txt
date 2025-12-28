cmake_minimum_required(VERSION 3.16)
project(temp2_testcases VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Math library
add_library(math_lib STATIC
    src/math/basic_math.cpp
    src/math/advanced_math.cpp
    src/math/statistics.cpp
)
target_include_directories(math_lib PUBLIC include)

# String utilities library
add_library(string_utils STATIC
    src/strings/string_utils.cpp
    src/strings/formatter.cpp
    src/strings/parser.cpp
)
target_include_directories(string_utils PUBLIC include)

# Data structures library
add_library(data_structures STATIC
    src/containers/linked_list.cpp
    src/containers/stack.cpp
    src/containers/queue.cpp
    src/containers/binary_tree.cpp
)
target_include_directories(data_structures PUBLIC include)

# Geometry library
add_library(geometry STATIC
    src/geometry/point.cpp
    src/geometry/vector2d.cpp
    src/geometry/shapes.cpp
    src/geometry/transforms.cpp
)
target_include_directories(geometry PUBLIC include)

# Algorithms library
add_library(algorithms STATIC
    src/algorithms/sorting.cpp
    src/algorithms/searching.cpp
    src/algorithms/graph.cpp
)
target_include_directories(algorithms PUBLIC include)

# Event system library
add_library(events STATIC
    src/events/event.cpp
    src/events/dispatcher.cpp
    src/events/handler.cpp
)
target_include_directories(events PUBLIC include)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE
    math_lib
    string_utils
    data_structures
    geometry
    algorithms
    events
)
