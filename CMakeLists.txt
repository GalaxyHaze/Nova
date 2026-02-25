cmake_minimum_required(VERSION 3.14)
project(Kalidous VERSION 0.1.0 LANGUAGES CXX)

# Create interface (header-only) library
add_library(Kalidous INTERFACE
        include/Kalidous/kalidous.h)
add_library(Kalidous::Kalidous ALIAS Kalidous)

target_include_directories(Kalidous
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(Kalidous INTERFACE cxx_std_17)

# Install headers + export target
include(GNUInstallDirs)

install(
        DIRECTORY   include/Kalidous
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        TARGETS     Kalidous
        EXPORT      KalidousTargets
        INCLUDES    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        EXPORT      KalidousTargets
        FILE        KalidousTargets.cmake
        NAMESPACE   Kalidous::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Kalidous
)

# Package configuration
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/KalidousConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/KalidousConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Kalidous
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/KalidousConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/KalidousConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/KalidousConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Kalidous
)