#include <delaunay_adaptive_cpu_interpolator.hpp>
#include "gtest/gtest.h"

class op : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state, std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0);
        values[1] = std::accumulate(state.begin(), state.end(), 1.0, std::multiplies<double>());
        return 0;
    }
};

TEST(DelaunayAdaptiveInterpolatorTest, Constructor){
    const int n_dim = 2;
    std::array<int, n_dim> axes_points{3, 4};
    std::array<double, n_dim> axes_min{0.0, 0.0};
    std::array<double, n_dim> axes_max{1.0, 1.2};
    op iface = op();
//    linear_static_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);
//    delaunay_static_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);
    delaunay_adaptive_cpu_interpolator<int, n_dim, 2> delaunay_interpolator(&iface, axes_points, axes_min, axes_max);

    EXPECT_EQ(1, 1);
}