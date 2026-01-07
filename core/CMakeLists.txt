# модуль "navigator_module"
pybind11_add_module(navigator_module
        src/bindings.cpp
        src/KalmanFilter.cpp
        src/Navigation.cpp
        src/Quaternion.cpp
        src/Vector.cpp
)

# вказуємо компілятору, де шукати заголовкові файли (.h)
target_include_directories(navigator_module PRIVATE imu_tracker)

find_package(Eigen3 REQUIRED)

# лінкуємо Eigen до модуля
target_link_libraries(navigator_module PRIVATE Eigen3::Eigen)