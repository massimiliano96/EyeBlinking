import os

from conans import ConanFile, tools
from conan.tools.cmake import CMake
from conan.tools.layout import cmake_layout


class EyeBlinkDetectorConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    # VirtualBuildEnv and VirtualRunEnv can be avoided if "tools.env.virtualenv:auto_use" is defined
    # (it will be defined in Conan 2.0)
    generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv", "VirtualRunEnv"
    apply_env = False

    generators = [
        "cmake"
    ]

    requires = [
        ("opencv/4.5.5"),
        ("dlib/19.24")
    ]

    tool_requires = "pkgconf/1.7.4"

    # Binary configuration

    default_options = {
        "opencv:shared": False,
        "opencv:with_ffmpeg": False,
        "opencv:with_cuda": False,
        "opencv:contrib": True,
        "opencv:cpu_baseline": "SSE2",

        "dlib:shared": False,
        "dlib:with_sse4": False,
        "dlib:with_avx": False
    }


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

