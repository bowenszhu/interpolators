#include <delaunay_adaptive_cpu_interpolator.h>
#include "gtest/gtest.h"

class SampleOperators : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state,
                 std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0);
        values[1] = std::accumulate(state.begin(), state.end(), 1.0,
                                    std::multiplies<double>());
        return 0;
    }
};

TEST(DelaunayInterpolatorTest, Integration) {
    const int n_dim = 2;
    const int n_op = 2;
    const std::array<int, n_dim> axes_points{3, 5};
    const std::array<double, n_dim> axes_min{0.0, 0.0};
    const std::array<double, n_dim> axes_max{1.0, 2.0};
    SampleOperators iface = SampleOperators();
    DelaunayAdaptiveCPUInterpolator<int, n_dim, n_op> delaunay_interpolator(
            &iface, axes_points, axes_min, axes_max);

    const std::vector<double> point{0.75, 1.725};
    std::vector<double> interpolation_values;
    delaunay_interpolator.Interpolate(point, interpolation_values);

    const int n_vertex = 1 << n_dim;
    // hypercube (square) vertices that enclose the test point
    std::array<std::vector<double>, n_vertex> hypercube_vertexes{
        {{0.5, 1.5}, {1.0, 1.5}, {0.5, 2.0}, {1.0, 2.0}}};
    std::array<std::array<double, n_vertex>, n_op> hypercube_vertex_values;

    for (int vertex_i = 0; vertex_i < n_vertex; vertex_i++) {
        std::vector<double> vertex_operator_values (n_op);
        iface.evaluate(hypercube_vertexes[vertex_i], vertex_operator_values);
        hypercube_vertex_values[0][vertex_i] = vertex_operator_values[0];
        hypercube_vertex_values[1][vertex_i] = vertex_operator_values[1];
    }

    for (int op_i = 0; op_i < n_op; op_i++) {
        const auto [min, max] = std::minmax_element(
                hypercube_vertex_values[op_i].begin(),
                hypercube_vertex_values[op_i].end());
        // test the interpolation result is >= the min of the hypercube vertex
        EXPECT_LE(*min, interpolation_values[op_i]);
        // test the interpolation result is <= the max of the hypercube vertex
        EXPECT_LE(interpolation_values[op_i], *max);
    }
}