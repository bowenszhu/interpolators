cmake_minimum_required(VERSION 3.17)
project(interpolators)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)

set(SOURCE_FILES main.cpp)
find_package(CGAL REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(interpolation_run ${SOURCE_FILES})
include_directories(src)
add_subdirectory(src)
target_link_libraries(interpolation_run interpolators_lib)
add_subdirectory(tests)


add_subdirectory(pybind11)
pybind11_add_module(interpolators
        src/python/py_main.cpp
        src/python/py_evaluator_iface.cpp
        src/python/py_operator_set_interpolator_pz.cpp
        )

target_link_libraries(interpolators PRIVATE CGAL::CGAL Eigen3::Eigen)
# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(interpolators PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
