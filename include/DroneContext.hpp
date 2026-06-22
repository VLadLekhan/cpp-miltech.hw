#pragma once


#include <memory>
#include "Types.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/ITargetProvider.hpp"

struct DroneContext {
    std::unique_ptr<IBallisticSolver> solver;
    std::unique_ptr<ITargetProvider> provider;

    float x = 0.0f;
    float y = 0.0f;
    float direction = 0.0f;     
    float currentSpeed = 0.0f;  
    float turnRemaining = 0.0f; 
    float targetDir = 0.0f;     
    float desiredDir = 0.0f;    
    int currentTargetIdx = 0;
    float currentTime = 0.0f;
    bool missionCompleted = false;

    DroneConfig cfg;
    
    static float normalizeAngle(float angle) {
    while (angle > M_PI) angle -= 2.0f * M_PI;
    while (angle < -M_PI) angle += 2.0f * M_PI;
    return angle;
}

};