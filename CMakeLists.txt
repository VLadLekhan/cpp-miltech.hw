cmake_minimum_required(VERSION 3.20)
project(cppmiltech_hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Зберіть тут усі ваші .cpp файли
set(SOURCES 
    src/main.cpp
    src/MissionProcessor.cpp
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
)


add_executable(cppmiltech_bin ${SOURCES})

# Додаємо лише корінь 'include'. 
# Тепер в коді ви просто пишете #include "interfaces/ITargetProvider.hpp"
target_include_directories(cppmiltech_bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)