cmake_minimum_required(VERSION 2.8)
project(matrix_module)

find_package(PythonLibs 3.6 REQUIRED)
python_add_module(matrix matrix.c)

include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(matrix ${PYTHON_LIBRARIES})
