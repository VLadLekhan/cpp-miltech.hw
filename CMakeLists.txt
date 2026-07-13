cmake_minimum_required(VERSION 3.20)
project(cppmiltech_hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES 
    src/main.cpp
    src/MissionProcessor.cpp
    src/DronePhysics.cpp
    src/providers/JsonTargetProvider.cpp
    src/solvers/AnalyticalSolver.cpp
    src/solvers/TableSolver.cpp
    src/config/FileConfigLoader.cpp
    src/config/ComponentFactory.cpp
    src/states/StateStopped.cpp
    src/states/StateAccelerating.cpp
    src/states/StateDeclareting.cpp
    src/states/StateTurning.cpp
    src/states/StateMoving.cpp
    src/drivers/uart_manager.cpp
    src/drivers/gpiod.cpp 
)

add_executable(cppmiltech_bin ${SOURCES})

target_include_directories(cppmiltech_bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)
target_link_libraries(cppmiltech_bin PRIVATE ${GPIOD_LIBRARIES})
target_include_directories(cppmiltech_bin PRIVATE ${GPIOD_INCLUDE_DIRS})