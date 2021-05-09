#ifndef BB750132_A4A2_43EF_AB87_90364057EDC8
#define BB750132_A4A2_43EF_AB87_90364057EDC8

#include <array>

#include "evaluator_iface.h"

/**
 * Interpolator base class
 */
template <typename index_t, int N_DIMS, int N_OPS>
class InterpolatorBase : public operator_set_gradient_evaluator_cpu
{
public:
    /**
     * @brief Evaluate all operators at the given state (point in parametrization space)
     *        Runs timer and calls virtual interpolate routine
     *
     * @param[in]   state   Coordinates in parametrization space
     * @param[out]  values  Interpolated values
     * @return 0 if evaluation is successful
     */
    int evaluate(const std::vector<double> &state, std::vector<double> &values);

    /**
     * @brief Evaluate operators and their gradient for every specified state (point in parametrization space)
     *
     * @param[in]   states        Array of coordinates in parametrization space
     * @param[in]   states_idxs   Indexes of states in the input array which are marked for evaluation
     * @param[out]  values        Interpolated values
     * @param[out]  derivatives   Interpolation gradients
     * @return 0 if evaluation is successful
     */
    int evaluate_with_derivatives(const std::vector<double> &states, const std::vector<int> &states_idxs,
                                  std::vector<double> &values, std::vector<double> &derivatives);

    /**
     * @brief Get the number of dimensions in interpolation space
     *
     */
    int get_n_dims() { return N_DIMS; };

    /**
     * @brief Get the number of operators to be interpolated
     *
     */
    int get_n_ops() { return N_OPS; };

    /**
     * @brief Get the number of supporting points for the given axis
     *
     * @param axis index of axis in question
     */
    int get_axis_n_points(int axis);

    /**
     * @brief Get the parametrization minimum value for given axis
     *
     * @param axis index of axis in question
     */
    double get_axis_min(int axis);

    /**
     * @brief Get the parametrization maximum value for given axis
     *
     * @param axis index of axis in question
     */
    double get_axis_max(int axis);

    /**
     * @brief Get the number of interpolations that took place
     *
     */
    uint64_t get_n_interpolations();

    /**
     * @brief Get the total number of supporting points in parameter space
     *
     * @return the total number of supporting points
     */
    uint64_t get_n_points_total();

    /**
     * @brief Get the number of supporting points used (evaluated through supporting_point_evaluator_)
     *        The number is equal to n_points_total_ for static interpolation methods
     *
     * @return the number of supporting points used
     */
    uint64_t get_n_points_used();

protected:
    /**
     * @brief Construct an interpolator with predefined parametrization space
     * 
     * @param[in] supporting_point_evaluator    Object used to compute operator values at supporting points
     * @param[in] axes_points                   Number of supporting points (minimum 2) along axes
     * @param[in] axes_min                      Minimum value for each axis
     * @param[in] axes_max                      Maximum for each axis
     */
    InterpolatorBase(operator_set_evaluator_iface *supporting_point_evaluator,
                     const std::array<int, N_DIMS> &axes_points,
                     const std::array<double, N_DIMS> &axes_min,
                     const std::array<double, N_DIMS> &axes_max);

    /**
     * @brief Given the coordinate of a point, the function computes the hypercube where the point is located and its scaled
     * coordinate inside the hypercube.
     *
     * @param[in] points The array of coordinates of points
     * @param[out] hypercube The lower-left vertex of the hypercube
     * @param[out] scaled_point The scaled coordinate of the given point inside the hypercube
     * @param[in] point_index Index of the point in the std::vector points
     *      The argument point_index is used only when std::vector points consists of multiple points.
     */
    void find_hypercube(const std::vector<double> &points, std::array<int, N_DIMS> &hypercube,
                        std::array<double, N_DIMS> &scaled_point, const int point_index = 0);

    /**
     * @brief Compute interpolation for all operators at the given point
     *
     * @param[in]   point   Coordinates in parametrization space
     * @param[out]  values  Interpolated values
     * @return 0 if interpolation is successful
     */
    virtual int Interpolate(const std::vector<double> &point, std::vector<double> &values) = 0;

    /**
     * @brief Compute interpolation and its gradient for all operators at every specified point
     *
     * @param[in]   points        Array of coordinates in parametrization space
     * @param[in]   points_idxs   Indexes of points in the points array which are marked for interpolation
     * @param[out]  values        Interpolated values
     * @param[out]  derivatives   Interpolation gradients
     * @return 0 if interpolation is successful
     */
    virtual int InterpolateWithDerivatives(const std::vector<double> &points, const std::vector<int> &points_idxs,
                                           std::vector<double> &values, std::vector<double> &derivatives) = 0;

    const std::array<int, N_DIMS> axes_points_;                       ///< number of supporting points along each axis
    const std::array<double, N_DIMS> axes_min_;                       ///< minimum at each axis
    const std::array<double, N_DIMS> axes_max_;                       ///< maximum of each axis
    operator_set_evaluator_iface *supporting_point_evaluator_; ///< object which computes operator values for supporting points

    std::array<double, N_DIMS> axes_step_;     ///< the distance between neighbor supporting points for each axis
    std::array<double, N_DIMS> axes_step_inv_; ///< inverse of step (to avoid division)

    std::array<index_t, N_DIMS> axes_mult_;                            /// multiplication factor used for transferring supporting point to point index

    uint64_t n_interpolations_; ///< Number of interpolations that took place
    uint64_t n_points_total_;   ///< Total number of parametrization points
    double n_points_total_fp_;  ///< Total number of parametrization points in floating point format, to detect index overflow in derived classes
    uint64_t n_points_used_;    ///< Number of parametrization points which were used (equal to n_points_total_ for static interpolators)

    /**
     * @brief This function transfers index of supporting point to its indices along axes.
     *
     * For example, in the 2-dimensional grid with axes_points_ = [4, 3],
     * the supporting point with index 0 is [0, 0],
     * the supporting point with index 2 is [0, 2],
     * the supporting point with index 4 is [1, 0],
     * the supporting point with index 5 is [1, 1],
     *
     * @param[in] index Index of the supporting point in the storage
     * @param[out] vertex The supporting point's index along each axis
     */
    void get_vertex_from_index(index_t index, std::array<int, N_DIMS> &vertex);

    /**
     * @brief Transfer a vertex to its coordinates.
     *
     * @param[in] vertex The indexes of the given supporting point along axes
     * @param[out] point The coordinates of the supporting point
     */
    void get_point_from_vertex(const std::array<int, N_DIMS> &vertex, std::vector<double> &point);

    /**
     * @brief Get values of operators at the given supporting point
     * Implementation depends on underlying storage. If static storage is used, the function simply reads
     * operator values of the given supporting point from the the storage.
     * If adaptive storage is used, the function checks whether the values were computed before,
     *      if yes, the value is directly returned;
     *      if not, the function computes the values, stores them and then returns.
     *
     * @param[in] vertex The indexes of coordinates the given supporting point along axes
     * @param[out] values The operator values at the given point
     */
    virtual void GetSupportingPoint(const std::array<int, N_DIMS> &vertex, std::array<double, N_OPS> &values) = 0;

    /**
     * @brief Given a supporting point, compute its index.
     *
     * This function is used as a hash for std::array<int, N_DIMS>.
     *
     * @param[in] vertex The indexes of coordinates the given supporting point along axes
     * @return The index of point among all supporting point
     */
    index_t get_index_from_vertex(const std::array<int, N_DIMS> &vertex);
};

#include "interpolator_base.tpp"
#endif /* BB750132_A4A2_43EF_AB87_90364057EDC8 */
