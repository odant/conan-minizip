# minizip Conan package
# Dmitriy Vetutnev, Odant 2019


from conans import ConanFile, CMake, tools
import os


class minizipConan(ConanFile):
    name = "minizip"
    version = "2.8.9"
    license = "ZLIB http://www.zlib.net/zlib_license.html"
    description = "minizip is a zip manipulation library written in C that is supported on Windows, macOS, and Linux."
    url = "https://github.com/odant/conan-minizip"
    settings = {
        "os": ["Windows", "Linux"],
        "compiler": ["Visual Studio", "gcc"],
        "build_type": ["Debug", "Release"],
        "arch": ["x86", "x86_64", "mips"]
    }
    options = {
        "with_unit_tests": [True, False]
    }
    default_options = "with_unit_tests=False"
    generators = "cmake"
    exports_sources = "src/*", "CMakeLists.txt", "FindMINIZIP.cmake"
    no_copy_source = True
    build_policy = "missing"

    def configure(self):
        # Pure C library
        del self.settings.compiler.libcxx

    def requirements(self):
        self.requires("zlib/1.2.11@%s/stable" % self.user)

    def build(self):
        build_type = "RelWithDebInfo" if self.settings.build_type == "Release" else "Debug"
        cmake = CMake(self, build_type=build_type, msbuild_verbosity='normal')
        cmake.verbose = True
        cmake.definitions["BUILD_SHARED_LIBS:BOOL"] = "OFF"
        #
        cmake.definitions["MZ_COMPAT"] = "ON"
        cmake.definitions["MZ_BZIP2"] = "OFF"
        cmake.definitions["SKIP_INSTALL_FILES:BOOL"] = "ON"
        cmake.definitions["SKIP_INSTALL_BINARIES:BOOL"] = "ON"
        if self.options.with_unit_tests:
            cmake.definitions["MZ_BUILD_TEST"] = "ON"
            cmake.definitions["MZ_BUILD_UNIT_TEST"] = "ON"
            cmake.definitions["MZ_BUILD_FUZZ_TEST"] = "ON"
        cmake.configure()
        cmake.build()
        if self.options.with_unit_tests:
            if cmake.is_multi_configuration:
                self.run("ctest --output-on-failure --build-config %s" % build_type)
            else:
                self.run("ctest --output-on-failure")
        cmake.install()

    def package(self):
        self.copy("FindMINIZIP.cmake", dst=".", src=".")
        self.copy("*minizip.pdb", dst="bin", src="src", keep_path=False)
        tools.rmdir(os.path.join(self.package_folder, "lib", "cmake"))

    def package_id(self):
        self.info.options.with_unit_tests = "any"

    def package_info(self):
        self.cpp_info.libs = ["minizip"]
        self.cpp_info.defines = ["HAVE_STDINT_H", "HAVE_INTTYPES_H"]
