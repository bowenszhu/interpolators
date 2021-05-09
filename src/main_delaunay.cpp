#include <iostream>
#include "evaluator_iface.h"
#include "delaunay_static_cpu_interpolator.hpp"
#include "linear_static_cpu_interpolator.hpp"
#include "delaunay_adaptive_cpu_interpolator.hpp"

class op : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state, std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0);
        values[1] = std::accumulate(state.begin(), state.end(), 1.0, std::multiplies<double>());
        return 0;
    }
};

int main() {
    const int n_dim = 2;
    std::array<int, n_dim> axes_points{3, 4};
    std::array<double, n_dim> axes_min{0.0, 0.0};
    std::array<double, n_dim> axes_max{1.0, 1.2};
    op iface = op();
//    linear_static_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);
//    delaunay_static_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);
    delaunay_adaptive_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);

    std::vector<double> point {0.6, 1.1};
    std::vector<int> points_idxs {0};
    std::vector<double> values;
    std::vector<double> derivatives;
    if (!delaunay_interpolator.interpolate_with_derivatives(point, points_idxs, values, derivatives))
//    if (!delaunay_interpolator.interpolate(point, values))
    {
        std::cout << "value      [0] = " << values[0]      << std::endl;
        std::cout << "value      [1] = " << values[1]      << std::endl;
        std::cout << "derivatives[0] = " << derivatives[0] << std::endl;
        std::cout << "derivatives[1] = " << derivatives[1] << std::endl;
        std::cout << "derivatives[2] = " << derivatives[2] << std::endl;
        std::cout << "derivatives[3] = " << derivatives[3] << std::endl;
    }
    else
        std::cout << "delaunay interpolation failed" << std::endl;

    return 0;
}

//int main() {
//    const int dim = 2;
//    const double pointsIn[][dim] = {
//            {0, 0},
//            {0, 1},
//            {1, 0},
//            {1, 1}
//    };
//    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<dim>>> T;
//    T dt(dim);
//    std::vector<T::Point> points;
//    int n_points = pow(2, dim);
//    points.reserve(n_points);
//    for (int j = 0; j < n_points; ++j) {
//        T::Point p(&pointsIn[j][0], &pointsIn[j][dim]);
//        points.push_back(p);
//    }
//    CGAL_assertion(dt.empty());
//    dt.insert(points.begin(), points.end());
//    CGAL_assertion(dt.is_valid());
//
//    const double testPointIn[dim] = {0.8, 0.9};
//    T::Point testPoint(&testPointIn[0], &testPointIn[dim]);
//    T::Full_cell_handle c = dt.locate(testPoint);
//    std::cout << "The point (";
//    for (int i = 0; i < dim; i++)
//        std::cout << " " << testPointIn[i] << " ";
//    std::cout << ") is located in the simplex with" << std::endl;
//
//    for (int vertex_i = 0; vertex_i <= dim; vertex_i++) {
//        T::Point vertex = c->vertex(vertex_i)->point();
//        std::cout << "vertex (";
//        for (int i = 0; i < dim; i++)
//            std::cout << " " << vertex[i] << " ";
//        std::cout << ")" << std::endl;
//    }
//
////    for (T::Full_cell::Vertex_handle_iterator v = c->vertices_begin(); v < c->vertices_end(); v++) {
////        T::Point vertex = (*v)->point();
////        std::cout << "vertex (";
////        for (int i = 0; i < dim; i++)
////            std::cout << " " << vertex[i] << " ";
////        std::cout << ")" << std::endl;
////    }
//
//    return 0;
//}
