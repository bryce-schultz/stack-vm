cmake_minimum_required(VERSION 3.10)

project(StackVM)
add_compile_options(-Wall -Wextra -Werror -pedantic -Wno-literal-suffix)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/compiler)
add_subdirectory(src/vm)