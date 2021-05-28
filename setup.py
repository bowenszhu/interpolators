# from setuptools import setup
#
# # Available at setup time due to pyproject.toml
# from pybind11.setup_helpers import Pybind11Extension, build_ext
# from pybind11 import get_cmake_dir
#
# import sys
#
# __version__ = "0.0.1"
#
# # The main interface is through Pybind11Extension.
# # * You can add cxx_std=11/14/17, and then build_ext can be removed.
# # * You can set include_pybind11=false to add the include directory yourself,
# #   say from a submodule.
# #
# # Note:
# #   Sort input source files if you glob sources to ensure bit-for-bit
# #   reproducible builds (https://github.com/pybind/python_example/pull/53)
#
# ext_modules = [
#     Pybind11Extension("interpolators",
#                       ["src/python/py_main.cpp",
#                        "src/python/py_evaluator_iface.cpp",
#                        "src/python/py_operator_set_interpolator_pz.cpp",
#                        "src/interpolator_base.cpp",
#                        "src/evaluator_iface.cpp"
#                        ],
#                       include_dirs=["src"],
#                       cxx_std=14,
#                       # Example: passing in the version to the compiled code
#                       define_macros=[('VERSION_INFO', __version__)],
#                       ),
# ]
#
# setup(
#     name="interpolators",
#     version=__version__,
#     # author="",
#     # author_email="",
#     # url="https://github.com/pybind/python_example",
#     description="A library of computationally efficient multidimensional multivariate interpolators based for CPU and GPU",
#     long_description="",
#     ext_modules=ext_modules,
#     # extras_require={"test": "pytest"},
#     # Currently, build_ext only provides an optional "highest supported C++
#     # level" feature, but in the future it may provide more features.
#     cmdclass={"build_ext": build_ext},
#     zip_safe=False,
# )



import os
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
        # from Python.
        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
            "-DPYTHON_EXECUTABLE={}".format(sys.executable),
            "-DEXAMPLE_VERSION_INFO={}".format(self.distribution.get_version()),
            "-DCMAKE_BUILD_TYPE={}".format(cfg),  # not used on MSVC, but no harm
        ]
        build_args = []

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do.
            # Users can override the generator with CMAKE_GENERATOR in CMake
            # 3.15+.
            if not cmake_generator:
                cmake_args += ["-GNinja"]

        else:

            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)
                ]
                build_args += ["--config", cfg]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only.
                build_args += ["-j{}".format(self.parallel)]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )


# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="interpolators",
    version="0.0.1",
    # author="",
    # author_email="",
    # url="https://github.com/pybind/python_example",
    description="A library of computationally efficient multidimensional multivariate interpolators based for CPU and GPU",
    long_description="",
    ext_modules=[CMakeExtension("interpolators")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
)
