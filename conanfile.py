from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class MachEmuRecipe(ConanFile):
    name = "mach_emu"
    version = "1.5.1"
    package_type = "library"
    test_package_folder = "Tests/ConanPackageTest"

    # Optional metadata
    license = "MIT"
    author = "Nicolas Beddows <nicolas.beddows@gmail.com>"
    url = "https://github.com/nbeddows"
    description = "8 bit Machine Emulator Engine"
    topics = ("emulator", "i8080")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "with_python": [True, False], "with_zlib": [True, False]}
    default_options = {"gtest*:build_gmock": False, "zlib*:shared": True, "shared": True, "fPIC": True, "with_python": False, "with_zlib": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt",\
        "Base/CMakeLists.txt",\
        "Base/include/*",\
        "Controller/CMakeLists.txt",\
        "Controller/include/*",\
        "Controller/source/*",\
        "Cpu/CMakeLists.txt",\
        "Cpu/include/*",\
        "Cpu/source/*",\
        "CpuClock/CMakeLists.txt",\
        "CpuClock/include/*",\
        "CpuClock/source/*",\
        "Machine/CMakeLists.txt",\
        "Machine/include/*",\
        "Machine/resource/*",\
        "Machine/source/*",\
        "MachinePy/CMakeLists.txt",\
        "MachinePy/include/*",\
        "MachinePy/source/*",\
        "Opt/CMakeLists.txt",\
        "Opt/include*",\
        "Opt/source/*",\
        "Sdk/CMakeLists.txt",\
        "SystemBus/CMakeLists.txt",\
        "SystemBus/include/*",\
        "Tests/CMakeLists.txt",\
        "Tests/ControllerTest/CMakeLists.txt",\
        "Tests/ControllerTest/source/*",\
        "Tests/MachineTest/CMakeLists.txt",\
        "Tests/MachineTest/source/*",\
        "Tests/Programs/*",\
        "Tests/TestControllers/CMakeLists.txt",\
        "Tests/TestControllers/include/*",\
        "Tests/TestControllersPy/CMakeLists.txt",\
        "Tests/TestControllersPy/source/*",\
        "Tests/TestControllers/source/*",\
        "Utils/CMakeLists.txt",\
        "Utils/include/*",\
        "Utils/source/*"

    def requirements(self):
        self.requires("base64/0.5.2")
        self.requires("gtest/1.14.0")
        self.requires("hash-library/8.0")
        self.requires("nlohmann_json/3.11.3")
        if self.options.with_python:
            self.requires("pybind11/2.12.0")
        if self.options.with_zlib:
            self.requires("zlib/1.3.1")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["enablePythonModule"] = self.options.with_python
        tc.variables["enableZlib"] = self.options.with_zlib
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):        
        self.cpp_info.libs = [self.name]
