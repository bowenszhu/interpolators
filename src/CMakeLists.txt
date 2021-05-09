cmake_minimum_required(VERSION 3.19)
project(delaunay2d)

set(CMAKE_CXX_STANDARD 20)

find_package(CGAL REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(delaunay
        main_delaunay.cpp
        evaluator_iface.cpp
        evaluator_iface.h
        interpolator_base.tpp
        interpolator_base.hpp
        linear_adaptive_cpu_interpolator.hpp
        linear_adaptive_cpu_interpolator.tpp
        linear_cpu_interpolator_base.hpp
        linear_cpu_interpolator_base.tpp
        linear_static_cpu_interpolator.hpp
        linear_static_cpu_interpolator.tpp
        timer_node.h
        delaunay_cpu_interpolator_base.hpp
        delaunay_cpu_interpolator_base.tpp
        static_cpu_interpolator_base.tpp
        static_cpu_interpolator_base.hpp
        adaptive_cpu_interpolator_base.tpp
        adaptive_cpu_interpolator_base.hpp
        delaunay_static_cpu_interpolator.tpp
        delaunay_static_cpu_interpolator.hpp)
target_link_libraries(delaunay CGAL::CGAL Eigen3::Eigen)

add_executable(linear
        main_linear.cpp
        evaluator_iface.cpp
        evaluator_iface.h
        interpolator_base.tpp
        interpolator_base.hpp
        linear_adaptive_cpu_interpolator.hpp
        linear_adaptive_cpu_interpolator.tpp
        linear_cpu_interpolator_base.hpp
        linear_cpu_interpolator_base.tpp
        linear_static_cpu_interpolator.hpp
        linear_static_cpu_interpolator.tpp
        timer_node.h
        delaunay_cpu_interpolator_base.hpp
        delaunay_cpu_interpolator_base.tpp
        static_cpu_interpolator_base.tpp
        static_cpu_interpolator_base.hpp
        adaptive_cpu_interpolator_base.tpp
        adaptive_cpu_interpolator_base.hpp)
target_link_libraries(linear CGAL::CGAL Eigen3::Eigen)