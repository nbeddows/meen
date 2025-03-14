# Copyright (c) 2021-2025 Nicolas Beddows <nicolas.beddows@gmail.com>

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.build import can_run
import os

class MeenRecipe(ConanFile):
    name = "meen"
    version = "2.0.0"
    package_type = "library"
    test_package_folder = "tests/conan_package_test"

    # Optional metadata
    license = "MIT"
    author = "Nicolas Beddows <nicolas.beddows@gmail.com>"
    url = "https://github.com/nbeddows"
    description = "8 bit Machine Emulator Engine"
    topics = ("emulator", "i8080")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "with_base64":[True, False], "with_i8080_test_suites": [True, False], "with_python": [True, False], "with_rp2040": [True, False], "with_save": [True, False], "with_zlib": [True, False]}
    default_options = {"zlib*:shared": True, "shared": True, "fPIC": True, "with_base64": True, "with_i8080_test_suites": False, "with_python": False, "with_rp2040": False, "with_save": True, "with_zlib": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    # "tests/CMakeLists.txt",\
    exports_sources = "CMakeLists.txt",\
        "CHANGELOG.md",\
        "LICENSE.md",\
        "README.md",\
        "docs/*",\
        "include/*",\
        "resource/*",\
        "source/*",\
        "tests/pythonTestDeps.cmake",\
        "tests/programs/*",\
        "tests/include/*",\
        "tests/source/*",

    def requirements(self):
        if self.options.get_safe("with_base64", False):
            self.requires("base64/0.5.2")

        if self.options.get_safe("with_save", False):
            self.requires("hash-library/8.0")

        if(self.settings.os == "baremetal"):
            self.requires("arduinojson/7.0.1")
        else:
            self.requires("nlohmann_json/3.11.3")

        if self.options.get_safe("with_python", False):
            self.requires("pybind11/2.12.0")
        if self.options.get_safe("with_zlib", False):
            self.requires("zlib/1.3.1")

    def build_requirements(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            if self.settings.os == "baremetal":
                self.test_requires("unity/2.6.0")
            else:
                self.test_requires("gtest/1.14.0")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

            if "arm" in self.settings.arch:
                self.output.error("Compiling for Windows ARM is not supported")

            if self.settings_build.os == "Linux" or self.settings_build == "baremetal":
                self.output.error("Cross compiling from Linux or baremetal to Windows is not supported")
        elif self.settings.os == "baremetal":
            self.output.info("base64 uri scheme and saving are not supported, removing options with_base64 and with_save")
            self.options.rm_safe("with_save")
            self.options.rm_safe("with_base64")

        if "arm" in self.settings.arch:
            self.output.info("Python ARM module not supported, removing option with_python.")
            self.options.rm_safe("with_python")

        if self.settings_build.os == "Windows":
            self.output.info("Cross compiling with RP2040 support from Windows is not supported, removing option with_rp2040")
            self.options.rm_safe("with_rp2040")

            if self.settings.os == "Linux" or self.settings.os == "baremetal":
                self.output.error("Cross compiling from Windows to Linux or baremetal is not supported")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
            self.options.rm_safe("with_rp2040")

        if not self.options.get_safe("with_save", False):
            self.options.rm_safe("with_zlib")

        if self.settings.os == "baremetal":
            if not self.options.get_safe("with_rp2040", False):
                self.output.error("Baremetal unsupported (did you enable 'with_rp2040'?)")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["enable_python_module"] = self.options.get_safe("with_python", False)
        tc.cache_variables["enable_zlib"] = self.options.get_safe("with_zlib", False)
        tc.cache_variables["enable_base64"] = self.options.get_safe("with_base64", False)
        tc.cache_variables["enable_hash_library"] = self.options.get_safe("with_save", False)
        tc.cache_variables["enable_rp2040"] = self.options.get_safe("with_rp2040", False)
        tc.variables["build_os"] = self.settings.os
        tc.variables["build_arch"] = self.settings.arch
        tc.variables["archive_dir"] = self.cpp_info.libdirs[0]
        tc.variables["runtime_dir"] = self.cpp_info.bindirs[0]
        if self.settings.os == "Windows" and self.options.get_safe("with_zlib", False) and self.dependencies["zlib"].options.shared:
            tc.variables["zlib_bin_dir"] = self.dependencies["zlib"].cpp_info.bindirs[0].replace("\\", "/")
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if can_run(self) and not self.conf.get("tools.build:skip_test", default=False):
            testFilter = "--gtest_filter=*"
            if not self.options.with_i8080_test_suites:
                testFilter += ":-*8080*:*CpuTest*"
            testsDir = os.path.join(self.source_folder, "artifacts", str(self.settings.build_type), str(self.settings.arch), self.cpp_info.bindirs[0])
            self.run(os.path.join(testsDir, "meen_test " + testFilter + " " + os.path.join(self.source_folder.replace("\\" ,"/") + "/tests/programs")))
            if self.options.get_safe("with_python", False):
                testFilter = "-k "
                if self.options.with_i8080_test_suites:
                    testFilter += "*"
                else:
                    testFilter += "MachineTest"
                cmd = os.path.join(self.source_folder, "tests/source/meen_test/test_Machine.py -v " + testFilter)
                self.run("python " + cmd)

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = [self.name]
