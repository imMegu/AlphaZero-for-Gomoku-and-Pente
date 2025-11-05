# Minimum required version for CMake
cmake_minimum_required(VERSION 3.14)

# Define your project name
project(Bot LANGUAGES CXX)

# Find dependencies
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# Define the target as a Python module (shared library)
pybind11_add_module(bot PythonConvert.cpp)


# Specify include directories for your header files
target_include_directories(bot PUBLIC 
    src
)

target_compile_options(bot PRIVATE 
    -O2
)

# Link the Pybind11 dependencies
target_link_libraries(bot PUBLIC pybind11::embed) 

# Ensure the module is compiled as a shared library
set_target_properties(bot PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.."
)

# This explicitly tells CMake what to do with main.cpp, preventing it from being
# accidentally compiled into the 'bot' module.
add_executable(gomoku_test src/main.cpp)

target_include_directories(gomoku_test PRIVATE
    src
)
