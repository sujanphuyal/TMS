cmake_minimum_required(VERSION 3.14)
project(TMS VERSION 1.0 DESCRIPTION "Time Management System")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find the necessary Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCE_FILES
    app/main.cpp
    src/views/Dashboard.cpp
    src/views/Scheduler.cpp
    src/views/HourTracker.cpp
    src/views/TimeClock.cpp
    app/TMSApp.cpp
)

# Define the executable
add_executable(TMS ${SOURCE_FILES}
    src/views/hourtracker.h src/views/hourtracker.cpp
    src/views/timeclock.h src/views/timeclock.cpp)

# Link libraries
target_link_libraries(TMS Qt6::Core Qt6::Gui Qt6::Widgets)
