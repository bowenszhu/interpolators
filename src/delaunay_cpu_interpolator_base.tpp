#include "delaunay_cpu_interpolator_base.hpp"
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>


template<typename index_t, int N_DIMS, int N_OPS>
delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::delaunay_cpu_interpolator_base(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::vector<int> &axes_points,
        const std::vector<double> &axes_min, const std::vector<double> &axes_max)
        : interpolator_base(supporting_point_evaluator, axes_points, axes_min, axes_max)
{
    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>> T;
//    delaunay_triangulation = T(N_DIMS);
    std::array<typename T::Point, 1 << N_DIMS> points; /// a hypercube has 2 ^ dim vertices in total
    for (int j = 0; j < points.size(); ++j) {
        std::bitset<N_DIMS> binary (j);
        double point[N_DIMS];
        for (int i = 0; i < N_DIMS; i++)
            point[i] = binary[i];
        typename T::Point p(&point[0], &point[N_DIMS]);
        points[j] = p;
    }
    CGAL_assertion(delaunay_triangulation.empty());
    delaunay_triangulation.insert(points.begin(), points.end());
    CGAL_assertion(delaunay_triangulation.is_valid());
}

template<typename index_t, int N_DIMS, int N_OPS>
int delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::interpolate(const std::vector<double> &point,
                                                                        std::vector<double> &values) {
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int
delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::interpolate_with_derivatives(const std::vector<double> &points,
                                                                                     const std::vector<int> &points_idxs,
                                                                                     std::vector<double> &values,
                                                                                     std::vector<double> &derivatives) {
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::get_n_dims() {
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::get_n_ops() {
    return 0;
}
