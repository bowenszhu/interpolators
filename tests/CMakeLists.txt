project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(interpolation_tests_run delaunay_test.cpp)

target_link_libraries(interpolation_tests_run interpolators_lib)
target_link_libraries(interpolation_tests_run CGAL::CGAL Eigen3::Eigen)

target_link_libraries(interpolation_tests_run gtest gtest_main)