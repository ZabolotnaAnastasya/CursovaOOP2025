cmake_minimum_required(VERSION 3.15)
project(prog)

set(CMAKE_CXX_STANDARD 20)

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

# завантажуємо pybind11
include(FetchContent)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.10.4
)
FetchContent_MakeAvailable(pybind11)

# підпапку core
add_subdirectory(core)

# куди покласти готовий файл .so
set_target_properties(navigator_module
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
)