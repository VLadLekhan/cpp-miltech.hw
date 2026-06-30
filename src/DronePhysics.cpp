#include "../include/DronePhysics.hpp"
#include <mutex>
#include <thread>
#include <chrono>

DronePhysics::DronePhysics(const DroneConfig& config) 
    : cfg(config), isRunning(false), x(0), y(0), vx(0), vy(0), timeSecSinceStart(0) {}

DronePhysics::~DronePhysics() {stop();}

void DronePhysics::start() {
    if (isRunning)  return; 

        isRunning = true;
        
     workerThread = std::thread(&DronePhysics::run, this);
    

}

void DronePhysics::stop() {
    isRunning = false;

    if(workerThread.joinable()){
        workerThread.join();
    }
}

bool DronePhysics::isThreadReady() const {
    return threadReady_;
}

void DronePhysics::run() {
    auto sleepDuration = std::chrono::milliseconds(static_cast<int>(cfg.physicsTimeStep * 1000));
    threadReady_ = true;
    
    while(isRunning) {
    processCommands();

    integrate(cfg.physicsTimeStep);

    timeSecSinceStart += cfg.physicsTimeStep;

    std::this_thread::sleep_for(sleepDuration);
    }
}

void DronePhysics::integrate(float dt) {
    std::lock_guard<std::mutex> lock(mtx);
    x += vx * dt;
    y += vy * dt;
    timeSecSinceStart += dt;
}

DroneTelemetry DronePhysics::getTelemetry(){
    std::lock_guard<std::mutex> lock(mtx);
    return {x, y, vx, vy, timeSecSinceStart};
}

void DronePhysics::sendCommand(DroneCommand cmd) {
    commandQueue.push(cmd);
}

void DronePhysics::applyCommand(const DroneCommand& cmd){
    std::lock_guard<std::mutex> lock(mtx);
    switch (cmd.state) {
        case DroneMode::ACCELERATING:
            break;
        case DroneMode::MOVING:
            break;
        case DroneMode::DECELERATING:
            break;
        case DroneMode::STOPPED:
            break;
        case DroneMode::TURNING:
            break;
        default:
            break;
    }

    // Застосовуємо вектор швидкості
    vx = cmd.targetVx;
    vy = cmd.targetVy;
}

void DronePhysics::processCommands() {
    DroneCommand cmd;

    while(commandQueue.try_pop(cmd)) {
        applyCommand(cmd);
    }
}