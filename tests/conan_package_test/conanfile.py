import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.build import can_run

class MachuEmuPackageTest(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)

        # Need to inform the python test app where the dependent shared libraries are located
        if self.dependencies["mach_emu"].options.get_safe("with_python", False):
            tc.variables["artifacts_dir"] = self.dependencies["mach_emu"].cpp_info.bindirs[0].replace("\\", "/")

            if self.settings.os == "Windows" and self.dependencies["mach_emu"].options.get_safe("with_zlib", False) and self.dependencies["zlib"].options.shared:
                tc.variables["zlib_bin_dir"] = self.dependencies["zlib"].cpp_info.bindirs[0].replace("\\", "/")
            tc.variables["MachEmuPackageTest"] = True

        tc.generate()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "MachEmuPackageTest")
            self.run(cmd, env="conanrun")

            # don't test the python module if we didn't configure it
            if self.dependencies["mach_emu"].options.with_python:
                cmd = os.path.join(self.source_folder, "source/MachEmuPackageTest.py")
                self.run("python " + cmd, env="conanrun")