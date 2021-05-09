project(interpolators_lib)

set(HEADER_FILES
        timer_node.h
        evaluator_iface.h
        interpolator_base.h
        static_cpu_interpolator_base.h
        adaptive_cpu_interpolator_base.h
        linear_cpu_interpolator_base.h
        delaunay_cpu_interpolator_base.h
        linear_static_cpu_interpolator.h
        linear_adaptive_cpu_interpolator.h
        delaunay_static_cpu_interpolator.h
        delaunay_adaptive_cpu_interpolator.h
        )

set(SOURCE_FILES
        evaluator_iface.cpp
        interpolator_base.tpp
        static_cpu_interpolator_base.tpp
        adaptive_cpu_interpolator_base.tpp
        linear_cpu_interpolator_base.tpp
        delaunay_cpu_interpolator_base.tpp
        linear_static_cpu_interpolator.tpp
        linear_adaptive_cpu_interpolator.tpp
        delaunay_static_cpu_interpolator.tpp
        delaunay_adaptive_cpu_interpolator.tpp
        )

add_library(interpolators_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})