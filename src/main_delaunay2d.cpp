#include <iostream>
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>

int main() {
    const int dim = 2;
    const double pointsIn[][dim] = {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
    };
    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<dim>>> T;
    T dt(dim);
    std::vector<T::Point> points;
    int n_points = pow(2, dim);
    points.reserve(n_points);
    for (int j = 0; j < n_points; ++j) {
        T::Point p(&pointsIn[j][0], &pointsIn[j][dim]);
        points.push_back(p);
    }
    CGAL_assertion(dt.empty());
    dt.insert(points.begin(), points.end());
    CGAL_assertion(dt.is_valid());

    const double testPointIn[dim] = {0.8, 0.9};
    T::Point testPoint(&testPointIn[0], &testPointIn[dim]);
    T::Full_cell_handle c = dt.locate(testPoint);
    std::cout << "The point (";
    for (int i = 0; i < dim; i++)
        std::cout << " " << testPointIn[i] << " ";
    std::cout << ") is located in the simplex with" << std::endl;
    for (T::Full_cell::Vertex_handle_iterator v = c->vertices_begin(); v < c->vertices_end(); v++) {
        T::Point vertex = (*v)->point();
        std::cout << "vertex (";
        for (int i = 0; i < dim; i++)
            std::cout << " " << vertex[i] << " ";
        std::cout << ")" << std::endl;
    }
    return 0;
}
