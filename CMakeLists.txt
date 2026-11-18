cmake_minimum_required(VERSION 3.16)
project(arm_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(arm_app
    src/main.cpp
    src/MainWindow.cpp
    src/Sensor.cpp
    src/Alert.cpp
    src/Zone.cpp
)

target_include_directories(arm_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(arm_app PRIVATE Qt6::Widgets)