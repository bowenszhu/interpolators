#include "delaunay_cpu_interpolator_base.hpp"
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>


template<typename index_t, int N_DIMS, int N_OPS>
delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::delaunay_cpu_interpolator_base(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::vector<int> &axes_points,
        const std::vector<double> &axes_min, const std::vector<double> &axes_max)
        : interpolator_base(supporting_point_evaluator, axes_points, axes_min, axes_max)
{
    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<dim>>> T;
    delaunay_triangulation = T(N_DIMS);
//    T delaunay_triangulation(N_DIMS);
    std::array<T::Point, 1 << N_DIMS> points; /// a hypercube has 2 ^ dim vertices in total
    for (int j = 0; j < points.size(); ++j) {
        std::bitset<dim> binary (j);
        double point[dim];
        for (int i = 0; i < dim; i++)
            point[i] = binary[i];
        T::Point p(&point[0], &point[dim]);
        points[j] = p;
    }
    CGAL_assertion(delaunay_triangulation.empty());
    delaunay_triangulation.insert(points.begin(), points.end());
    CGAL_assertion(delaunay_triangulation.is_valid());
}