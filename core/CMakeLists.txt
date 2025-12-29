# 1. Створюємо модуль "navigator_module"
# Тут ми перераховуємо всі .cpp файли з папки src/
pybind11_add_module(navigator_module
        src/bindings.cpp
        src/KalmanFilter.cpp
        src/Navigation.cpp
        src/Quaternion.cpp
        src/Vector.cpp
)

# 2. Вказуємо компілятору, де шукати заголовкові файли (.h)
# Вони лежать у папці imu_tracker поруч із цим CMakeLists.txt
target_include_directories(navigator_module PRIVATE imu_tracker)

find_package(Eigen3 REQUIRED)

# 4. Лінкуємо Eigen до нашого модуля
target_link_libraries(navigator_module PRIVATE Eigen3::Eigen)