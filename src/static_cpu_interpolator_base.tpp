#include "static_cpu_interpolator_base.h"


template<typename index_t, int N_DIMS, int N_OPS>
StaticCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::StaticCPUInterpolatorBase(
        operator_set_evaluator_iface *supportingPointEvaluator, const std::array<int, N_DIMS> &axesPoints,
        const std::array<double, N_DIMS> &axesMin, const std::array<double, N_DIMS> &axesMax)
        : InterpolatorBase<index_t, N_DIMS, N_OPS>(supportingPointEvaluator, axesPoints, axesMin, axesMax)
{
    std::cout << "Computing " << this->n_points_total_ << " supporting points for static storage..." << std::endl;
    point_data_.resize(this->n_points_total_ * N_OPS);

    for (index_t point_i = 0; point_i < this->n_points_total_; point_i++)
    {
        std::array<int, N_DIMS> vertex;
        InterpolatorBase<index_t, N_DIMS, N_OPS>::GetVertexFromIndex(point_i, vertex);

        std::vector<double> new_point_coords (N_DIMS);
        InterpolatorBase<index_t, N_DIMS, N_OPS>::GetPointFromVertex(vertex, new_point_coords);

        std::vector<double> new_operator_values (N_OPS);
        this->supporting_point_evaluator_->evaluate(new_point_coords, new_operator_values);

        std::copy(new_operator_values.begin(), new_operator_values.end(), point_data_.begin() + N_OPS * point_i);
    }
}

template <typename index_t, int N_DIMS, int N_OPS>
void StaticCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::GetSupportingPoint(const std::array<int, N_DIMS> &vertex,
                                                                           std::array<double, N_OPS> &values)
{
    index_t index = this->GetIndexFromVertex(vertex);

    std::copy(point_data_.begin() + index * N_OPS, point_data_.begin() + (index + 1) * N_OPS, values.begin());
}