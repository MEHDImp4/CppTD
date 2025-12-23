 cmake_minimum_required(VERSION 3.16)

project(CppTD_Qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/Measurement.cpp
    src/WeatherStation.cpp
    src/Analyzer.cpp
    src/MainWindow.cpp
    src/AddMeasurementDialog.cpp
)

# Check if implementation files exist, otherwise we might need to adjust
# For now assuming standard separation.

add_executable(CppTD_Qt ${SOURCES})

target_link_libraries(CppTD_Qt PRIVATE Qt6::Widgets)

# Handle Windows specific settings to avoid console window if desired (optional)
# set_target_properties(CppTD_Qt PROPERTIES WIN32_EXECUTABLE ON)
