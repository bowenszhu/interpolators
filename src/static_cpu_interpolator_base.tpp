#include "static_cpu_interpolator_base.hpp"


template<typename index_t, int N_DIMS, int N_OPS>
static_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::static_cpu_interpolator_base(
        operator_set_evaluator_iface *supportingPointEvaluator, const std::array<int, N_DIMS> &axesPoints,
        const std::array<double, N_DIMS> &axesMin, const std::array<double, N_DIMS> &axesMax)
        : InterpolatorBase<index_t, N_DIMS, N_OPS>(supportingPointEvaluator, axesPoints, axesMin, axesMax)
{
    std::cout << "Computing " << this->n_points_total << " supporting points for static storage..." << std::endl;
    point_data.resize(this->n_points_total * N_OPS);

    for (index_t point_i = 0; point_i < this->n_points_total; point_i++)
    {
        std::array<int, N_DIMS> vertex;
        InterpolatorBase<index_t, N_DIMS, N_OPS>::get_vertex_from_index(point_i, vertex);

        std::vector<double> new_point_coords (N_DIMS);
        InterpolatorBase<index_t, N_DIMS, N_OPS>::get_point_from_vertex(vertex, new_point_coords);

        std::vector<double> new_operator_values (N_OPS);
        this->supporting_point_evaluator->evaluate(new_point_coords, new_operator_values);

        std::copy(new_operator_values.begin(), new_operator_values.end(), point_data.begin() + N_OPS * point_i);
    }
}

template <typename index_t, int N_DIMS, int N_OPS>
void static_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::get_supporting_point(const std::array<int, N_DIMS> &vertex,
                                                                                std::array<double, N_OPS> &values)
{
    index_t index = this->get_index_from_vertex(vertex);

    std::copy(point_data.begin() + index * N_OPS, point_data.begin() + (index + 1) * N_OPS, values.begin());
}