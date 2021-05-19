#ifndef DELAUNAY_CPU_INTERPOLATOR_BASE_HPP
#define DELAUNAY_CPU_INTERPOLATOR_BASE_HPP


#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>
#include "interpolator_base.h"

/**
 * @brief  Interpolator base for static/adaptive barycentric interpolator
 *         with Delaunay triangulation.
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template<typename index_t, int N_DIMS, int N_OPS>
class DelaunayCPUInterpolatorBase
        : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS> {
public:
    /**
     * @brief Compute interpolation for all operators at the given point
     *
     * @param[in]   point   Coordinates in parametrization space
     * @param[out]  values  Interpolated values
     * @return 0 if interpolation is successful
     */
    int Interpolate(const std::vector<double> &point,
                    std::vector<double> &values) override;

    /**
     * @brief Compute interpolation and its gradient for all operators at
     *        every specified point
     *
     * @param[in]   points        Array of coordinates in parametrization space
     * @param[in]   points_idxs   Indexes of points in the points array which
     *                            are marked for interpolation
     * @param[out]  values        Interpolated values
     * @param[out]  derivatives   Interpolation gradients
     * @return 0 if interpolation is successful
     */
    int InterpolateWithDerivatives(const std::vector<double> &points,
                                   const std::vector<int> &points_idxs,
                                   std::vector<double> &values,
                                   std::vector<double> &derivatives) override;

protected:
    /**
     * @brief Construct an interpolator with specified parametrization space
     *
     * @param[in] supporting_point_evaluator Object used to compute operators
     *                                       values at supporting points
     * @param[in] axes_points Number of supporting points (minimum 2) along axes
     * @param[in] axes_min Minimum value for each axis
     * @param[in] axes_max Maximum for each axis
     */
    DelaunayCPUInterpolatorBase(
            operator_set_evaluator_iface *supporting_point_evaluator,
            const std::array<int, N_DIMS> &axes_points,
            const std::array<double, N_DIMS> &axes_min,
            const std::array<double, N_DIMS> &axes_max);

    /// CGAL multi-dimensional Delaunay triangulation object
    CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>>
            delaunay_triangulation_{N_DIMS};

    /**
     * @brief Find the simplex for the given point in the given hypercube based
     *        on Delaunay triangulation. Construct the square vertex_matrix of
     *        size ((N_DIMS + 1) * (N_DIMS + 1)).
     *        For a simplex in 2d space with ordered vertices
     *        [0, 0], [1, 0], [1, 1], the vertex_matrix will be
     *        [[0, 1, 1],
     *         [0, 0, 1],
     *         [1, 1, 1]]
     *        where the elements in the first two rows and the first column
     *        corresponds to the first vertex [0, 0],
     *        the elements in the first two rows and the second column
     *        corresponds to the second vertex [1, 0], and
     *        the elements in the first two rows and the last column
     *        corresponds to the last vertex [0, 0].
     *        The last row are all ones.
     *
     * @param[in] hypercube      The lower-left vertex of the hypercube in
     *                           indexed supporting points
     * @param[in] scaled_point   The scaled coordinate of the given point
     *                           inside the hypercube
     * @param[out] vertex_matrix Matrix of simplex vertices
     * @param[out] simplex       Array of vertices of the simplex
     */
    void FindSimplex(const std::array<int, N_DIMS> &hypercube,
                     const std::array<double, N_DIMS> &scaled_point,
                     Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
                     std::array<std::array<int, N_DIMS>, N_DIMS + 1> &simplex);

    /**
     * @brief Compute barycentric coordinate by solving the linear system
     *        A x = b
     *        where A is the vertex matrix, b is the scaled point,
     *        x is the barycentric coordinate.
     *        Eigen::ColPivHouseholderQR is used for linear solving.
     *
     * @param[in] vertex_matrix Square matrix of simplex vertices as columns
     * @param[in] scaled_point Scaled coordinate of the given point
     *                         inside its hypercube
     * @param[out] weights Barycentric coordinate of the point in the simplex
     */
    void ComputeBarycentricCoordinates(
            const Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
            const std::array<double, N_DIMS> &scaled_point,
            Eigen::Matrix<double, N_DIMS + 1, 1> &weights);

    /**
     * @brief Compute the inverse of the vertex matrix.
     *        Compute barycentric coordinate by multiply the inverse matrix
     *        with the column vector of the scaled point.
     *
     * @param[in] vertex_matrix Square matrix of simplex vertices as columns
     * @param[in] scaled_point Scaled coordinate of the given point
     *                         inside its hypercube
     * @param[out] weights Barycentric coordinate of the point in the simplex
     * @param[out] vertex_matrix_inv Inverse of the vertex matrix
     */
    void ComputeBarycentricCoordinatesAndVertexMatrixInv(
            const Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
            const std::array<double, N_DIMS> &scaled_point,
            Eigen::Matrix<double, N_DIMS + 1, 1> &weights,
            Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix_inv);
};

#include "delaunay_cpu_interpolator_base.tpp"

#endif //DELAUNAY_CPU_INTERPOLATOR_BASE_HPP
