cmake_minimum_required(VERSION 3.19)
project(delaunay2d)

set(CMAKE_CXX_STANDARD 20)

find_package(CGAL)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(delaunay2d main_delaunay2d.cpp)
target_link_libraries(delaunay2d CGAL::CGAL Eigen3::Eigen)