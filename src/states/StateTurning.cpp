#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"
#include <iostream>

std::unique_ptr<IDroneState> StateTurning::execute(DroneContext& ctx) {

    ctx.currentAccel = 0.0f;


    
    float targetAngle = std::atan2(ctx.targets.y- ctx.telemetry.y, ctx.targets.x- ctx.telemetry.x);

    float delta = DroneContext::normalizeAngle(targetAngle - ctx.telemetry.dir);
    float rotationStep = ctx.config.timeStep * ctx.config.angularSpeed;

    if (std::abs(delta) <= rotationStep) { 
        return std::make_unique<StateAccelerating>();
    }

    ctx.currentTurnRate = (delta > 0.0f) ? 1.0f : -1.0f;

   return nullptr;  
}

float StateTurning::estimateTimeToChange(const DroneContext& ctx) {
     float targetAngle = std::atan2(ctx.targets.y- ctx.telemetry.y, ctx.targets.x- ctx.telemetry.x);
    float diff_angle = DroneContext::normalizeAngle(targetAngle - ctx.telemetry.dir);

    return (std::abs(diff_angle) / ctx.config.angularSpeed);
}

bool StateTurning::isFinished() const {
    return false; 
}