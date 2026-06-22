#include <iostream>
#include <memory>

#include "../include/MissionProcessor.hpp"

#include "../include/config/FileConfigLoader.hpp"
#include "../include/providers/JsonTargetProvider.hpp"
#include "../include/solvers/AnalyticalSolver.hpp"
#include "../include/solvers/TableSolver.hpp"

#include "../include/states/StateStopped.hpp"
#include "../include/states/StateMoving.hpp"
#include "../include/states/StateTurning.hpp"

int main() {
    std::cout << "=== Наземна станція керування: Старт місії ===" << std::endl;

    auto analyticalSolver = std::make_unique<AnalyticalSolver>();
    auto tableSolver = std::make_unique<TableSolver>("data/ballistic_table.txt");
    auto loader = std::make_unique<FileConfigLoader>("data/config.json");
    auto provider = std::make_unique<JsonTargetProvider>("data/targets.json");

    MissionProcessor processor (std::move(tableSolver), std::move(loader), std::move(provider));

    processor.init(std::make_unique<StateStopped>());


    std::cout << "\nЗапуск покрокового розрахунку балістики..." << std::endl;

    int counter = 0;
    const int maxSteps = 1000;

    while (counter < maxSteps) {
    processor.step();

    if (processor.getContext().missionCompleted) {
             std::cout << "[LOG] Бомбу скинуто. Місія успішна!" << std::endl;
             break;
        }

    std::cout << "DEBUG: Час: " << processor.getContext().currentTime 
          << " | Швидкість: " << processor.getContext().currentSpeed 
          << " | Координати: (" << processor.getContext().x << ", " << processor.getContext().y << ")" << std::endl;

    counter++;
}

if (counter >= maxSteps) {
    std::cout << "ПОМИЛКА: Місія не завершилася за 1000 кроків!" << std::endl;
}

    std::cout << "\n=============================================" << std::endl;

    std::cout << "=== Місія завершена успішно ===" << std::endl;
    return 0;
}

