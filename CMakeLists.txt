cmake_minimum_required(VERSION 3.10)

project(StackVM)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/compiler)
add_subdirectory(src/vm)