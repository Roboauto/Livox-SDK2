import os

from conans import ConanFile, CMake, tools
# from conan.tools.cmake import CMakeToolchain, CMake

from pprint import pformat


class LivoxLidarSDKConan(ConanFile):
    name = "livox_lidar_sdk"
    version = "1.2.4.7"
    license = "MIT"
    url = "https://github.com/Livox-SDK/Livox-SDK2"
    description = "Drivers for receiving LiDAR data and controlling lidar, support Lidar HAP and Mid-360"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "cmake_paths", "cmake_find_package"
    exports_sources = [
        "3rdparty/*",
        "include/*",
        "sdk_core/*",
        "samples/*",
        "sdk_core/*",
        "CMakeLists.txt",
        "LICENSE.txt",
        "README.md"]

    # def requirements(self):
        # self.requires("fmt/10.2.1")
        # self.requires("spdlog/1.13.0")
        # self.requires("rapidjson/cci.20230929")

    # def configure(self):
    #     self.options["spdlog"].spdlog_fmt_external = True

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions[
            "CMAKE_PROJECT_livox_lidar_sdk_INCLUDE"] = os.path.join(
            self.build_folder, "conan_paths.cmake")
        cmake.definitions["BUILD_SHARED_LIBS"] = True if self.options.shared else False

        self.output.info("Cmake definitions: ")
        self.output.info(pformat(cmake.definitions))
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.includedirs = [
            "include"]

        self.cpp_info.filenames["cmake_find_package"] = "livox_lidar_sdk"
        self.cpp_info.filenames["cmake_find_package_multi"] = "livox_lidar_sdk"
        self.cpp_info.names["cmake_find_package"] = "livox_lidar_sdk"
        self.cpp_info.names["cmake_find_package_multi"] = "livox_lidar_sdk"
