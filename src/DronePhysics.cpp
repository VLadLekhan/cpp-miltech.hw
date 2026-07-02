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
    lastTick_ = std::chrono::steady_clock::now();


    while(isRunning) {
        processCommands();

        auto now = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(now - lastTick_).count();
        lastTick_ = now;

        if (dt > 0.25f) dt = cfg.physicsTimeStep;

        integrate(dt);

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
    return {x, y, vx, vy, timeSecSinceStart, currentMode_};
}

void DronePhysics::sendCommand(DroneCommand cmd) {
    commandQueue.push(cmd);
}

void DronePhysics::applyCommand(const DroneCommand& cmd){
    std::lock_guard<std::mutex> lock(mtx);
    currentMode_ = cmd.state;
    vx = cmd.targetVx;
    vy = cmd.targetVy;
}

void DronePhysics::processCommands() {
    DroneCommand cmd;

    while(commandQueue.try_pop(cmd)) {
        applyCommand(cmd);
    }
}