cmake_minimum_required(VERSION 3.16)
project(WeatherStation VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable Qt MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files (without main files)
set(COMMON_SOURCES
    src/Measurement.cpp
    src/WeatherStation.cpp
    src/Analyzer.cpp
)

# Console application (original)
add_executable(weather_station_console
    ${COMMON_SOURCES}
    src/main.cpp
)

# Qt GUI application
add_executable(weather_station_qt
    ${COMMON_SOURCES}
    src/MainWindow.cpp
    src/main_qt.cpp
)

# Link Qt libraries to the Qt executable
target_link_libraries(weather_station_qt PRIVATE Qt6::Widgets)

# Set Windows-specific properties for Qt app (hide console window)
if(WIN32)
    set_target_properties(weather_station_qt PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
