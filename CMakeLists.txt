cmake_minimum_required(VERSION 3.17)
project(interpolators)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)

find_package(CGAL REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(SOURCE_FILES main.cpp)
add_executable(interpolation_run ${SOURCE_FILES})

include_directories(src)
add_subdirectory(src)

target_link_libraries(interpolation_run interpolators_lib)

add_subdirectory(tests)