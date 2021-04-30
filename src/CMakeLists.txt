cmake_minimum_required(VERSION 3.19)
project(delaunay2d)

set(CMAKE_CXX_STANDARD 20)

find_package(CGAL REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(delaunay2d
        main_delaunay2d.cpp
        evaluator_iface.cpp
        evaluator_iface.h
        interpolator_base.cpp
        interpolator_base.hpp
        linear_adaptive_cpu_interpolator.hpp
        linear_adaptive_cpu_interpolator.tpp
        linear_cpu_interpolator_base.hpp
        linear_cpu_interpolator_base.tpp
        linear_static_cpu_interpolator.hpp
        linear_static_cpu_interpolator.tpp
        timer_node.h
        delaunay_cpu_interpolator_base.hpp
        delaunay_cpu_interpolator_base.tpp)
target_link_libraries(delaunay2d CGAL::CGAL Eigen3::Eigen)