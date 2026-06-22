#pragma once

#include "ThreadSafeQueue.hpp"
#include "Types.hpp"
#include<thread>
#include<atomic>
#include<mutex>


class DronePhysics {
    private:
    float x, y, vx, vy;
    float timeSecSinceStart;

    const DroneConfig cfg;

    std::thread workerThread;
    std::atomic<bool> isRunning;

    mutable std::mutex mtx;
    ThreadSafeQueue<DroneCommand> commandQueue;

    void run();             
    void integrate(float dt); 
    void processCommands();

    public:
    DronePhysics(const DroneConfig& config);
    ~DronePhysics();

    void start();
    void stop();

    DroneTelemetry getTelemetry();
    void sendCommand(DroneCommand cmd);

};