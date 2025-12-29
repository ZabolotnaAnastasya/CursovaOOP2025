cmake_minimum_required(VERSION 3.15)
project(prog)

set(CMAKE_CXX_STANDARD 20)

# 1. Знаходимо Python (для роботи pybind11)
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

# 2. Завантажуємо pybind11 (інструмент для створення модуля)
include(FetchContent)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.10.4
)
FetchContent_MakeAvailable(pybind11)

# 3. Додаємо твою підпапку core
add_subdirectory(core)

# 4. Вказуємо, куди покласти готовий файл .so
# (Він з'явиться у твоїй папці python/, поруч з main.py)
set_target_properties(navigator_module
        PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
)