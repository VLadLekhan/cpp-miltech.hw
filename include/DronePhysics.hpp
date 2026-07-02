#pragma once

#include "ThreadSafeQueue.hpp"
#include "Types.hpp"
#include<thread>
#include<atomic>
#include<mutex>
#include<chrono>


class DronePhysics {
    private:
    float x, y, vx, vy;
    float timeSecSinceStart;

    const DroneConfig cfg;

    std::thread workerThread;
    std::atomic<bool> isRunning;
    std::atomic<bool> threadReady_{false};
    mutable std::mutex mtx;
    ThreadSafeQueue<DroneCommand> commandQueue;
    std::chrono::steady_clock::time_point lastTick_;
    DroneMode currentMode_ = DroneMode::STOPPED;

                
    void integrate(float dt); 
    void processCommands();
    void applyCommand(const DroneCommand& cmd);

    public:
    DronePhysics(const DroneConfig& config);
    ~DronePhysics();
    
    bool isThreadReady() const;
    void run(); 
    void start();
    void stop();

    DroneTelemetry getTelemetry();
    void sendCommand(DroneCommand cmd);

};