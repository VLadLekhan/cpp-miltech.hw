#include <iostream>
#include <memory>
#include <thread>

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
    auto provider = std::make_shared<JsonTargetProvider>("data/targets.json");
    auto physics = std::make_shared<DronePhysics>(loader->getConfig());

    MissionProcessor processor (std::move(tableSolver), std::move(loader), provider, physics);
    processor.init(std::make_unique<StateStopped>());

    std::thread providerThreat(&JsonTargetProvider::run, provider.get());
    std::thread physicsThread(&DronePhysics::run, physics.get());
	std::thread missionThread(&MissionProcessor::run, &processor);



    std::cout << "\nЗапуск покрокового розрахунку балістики..." << std::endl;

    while (!provider->isThreadReady() || !physics->isThreadReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // 5. Старт симуляції
    provider->start();
    physics->start();
    // processor.start(); // Якщо в місії є свій прапорець start()

    // 6. Очікування завершення МІСІЇ
    missionThread.join();

    // 7. Зупинка сервісних потоків
    physics->stop();
    provider->stop();

    providerThreat.join();
    physicsThread.join();

    std::cout << "=== Місія завершена успішно ===" << std::endl;
    return 0;
}

