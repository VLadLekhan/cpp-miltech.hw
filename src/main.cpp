#include <iostream>
#include <cstring>
#include <chrono>
#include <memory>
#include <thread>

#include "../include/MissionProcessor.hpp"
#include "../include/drivers/uart_manager.hpp"
#include "../include/drivers/gpiod.hpp"
#include "../include/solvers/AnalyticalSolver.hpp"
#include "../include/states/StateStopped.hpp"


int main(int argc, char** argv) {
    std::cout << "=== Наземна станція керування: Старт місії ===" << std::endl;

    std::string uartDev = "/tmp/ttyA";
    std::string gpiochip = "gpiochip";
    int startLine = 24;
    int dropLine = 23;

    for(int i = 1; i < argc; ++i) {
       if (!strcmp(argv[i], "--uart") && i+1 < argc) uartDev = argv[++i];
        else if (!strcmp(argv[i], "--gpiochip") && i+1 < argc) gpiochip = argv[++i];
        else if (!strcmp(argv[i], "--start-line") && i+1 < argc) startLine = std::atoi(argv[++i]);
        else if (!strcmp(argv[i], "--drop-line") && i+1 < argc) dropLine = std::atoi(argv[++i]); 
    }

    std::cout << "=== Автопілот: старт ===" << std::endl;

    UartManager uart(uartDev.c_str());
    GpioManager gpio(gpiochip.c_str(), startLine,  dropLine);

    uart.update();
    while (uart.getAmmo().nTargets == 0) {
        uart.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "[LOG] Отримано перші дані від чекера, стартуємо місію" << std::endl;

    auto solver = std::make_unique<AnalyticalSolver>();
    MissionProcessor processor (std::move(solver), &uart, &gpio );
    processor.init(std::make_unique<StateStopped>());

    processor.run();

    std::cout << "=== Місія завершена ===" << std::endl;
    return 0;

}

