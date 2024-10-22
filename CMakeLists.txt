# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(NewLinux)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
# Ensure that the C++ standard is required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure PkgConfig is available for finding packages
find_package(PkgConfig REQUIRED)

# Find the GTK3 package using PkgConfig
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include directories for GTK3 headers
include_directories(${GTK3_INCLUDE_DIRS})

# Link directories for GTK3 libraries
link_directories(${GTK3_LIBRARY_DIRS})

# Create the executable from the main source file located in src
add_executable(newlinux src/main.cpp)

# Link the executable with GTK3 libraries
target_link_libraries(newlinux ${GTK3_LIBRARIES})

# Set the package name for CPack (CMake packaging)
set(CPACK_PACKAGE_NAME "NewLinux")
# Set the package version for CPack
set(CPACK_PACKAGE_VERSION "1.0.0")
# Set the package description for CPack
set(CPACK_PACKAGE_DESCRIPTION "NewLinux is a kernel update tool.")
# Set the vendor for the package in CPack
set(CPACK_PACKAGE_VENDOR "Voltaged")
# Set the maintainer for Debian package
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Voltaged <rusindanilo@gmail.com>")

# Include CPack for packaging functionalities
include(CPack)
