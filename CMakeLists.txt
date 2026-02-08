cmake_minimum_required(VERSION 3.14)
project(Nova VERSION 0.1.0 LANGUAGES CXX)

# Create interface (header-only) library
add_library(Nova INTERFACE
        include/Nova/nova.h)
add_library(Nova::Nova ALIAS Nova)

target_include_directories(Nova
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(Nova INTERFACE cxx_std_17)

# Install headers + export target
include(GNUInstallDirs)

install(
        DIRECTORY   include/Nova
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        TARGETS     Nova
        EXPORT      NovaTargets
        INCLUDES    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        EXPORT      NovaTargets
        FILE        NovaTargets.cmake
        NAMESPACE   Nova::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nova
)

# Package configuration
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/NovaConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/NovaConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nova
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/NovaConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NovaConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/NovaConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Nova
)