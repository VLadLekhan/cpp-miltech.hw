#pragma once


#include <memory>
#include <vector>
#include "Types.hpp"
#include "drone_link.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/ITargetProvider.hpp"
#include "ThreadSafeQueue.hpp"
#include "DronePhysics.hpp"

struct DroneContext {
    std::unique_ptr<IBallisticSolver> solver;

    dlink::Telemetry telemetry;
    dlink::AmmoCfg ammo;
    dlink::TargetPos targets;
    dlink::DroneCfg config;
    std::vector<dlink::TargetPos> allTargets;
    
    bool missionCompleted = false;
    int lastProcessedTargetIdx = -1;
    float currentAccel = 0.0f;
    float currentTurnRate = 0.0f;
    int currentTargetIdx = 0;
    bool dropRequested = false;
    ThreadSafeQueue<DroneCommand> commandQueue;

    // float x = 0.0f;
    // float y = 0.0f;
    // float direction = 0.0f;     
    // float currentSpeed = 0.0f; 
     
    // float turnRemaining = 0.0f; 
    // float targetDir = 0.0f;     
    // float desiredDir = 0.0f;    
    
    
    

    
    static float normalizeAngle(float angle) {
    while (angle > M_PI) angle -= 2.0f * M_PI;
    while (angle < -M_PI) angle += 2.0f * M_PI;
    return angle;
}

};