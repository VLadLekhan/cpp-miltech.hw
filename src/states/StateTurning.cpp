#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"
#include <iostream>

std::unique_ptr<IDroneState> StateTurning::execute(DroneContext& ctx) {
    Target targetPos = ctx.provider->getTarget(ctx.currentTargetIdx);
    float targetAngle = std::atan2(targetPos.pos.y - ctx.y, targetPos.pos.x - ctx.x);
    ctx.desiredDir = std::atan2(targetPos.pos.y - ctx.y, targetPos.pos.x - ctx.x);

    float delta = DroneContext::normalizeAngle(ctx.desiredDir - ctx.direction);
    float rotationStep = ctx.cfg.physicsTimeStep * ctx.cfg.angularspeed;

    if (std::abs(delta) <= rotationStep) { 
        ctx.direction = targetAngle; // Фіксуємо точний кут
        return std::make_unique<StateAccelerating>();
    }

    DroneCommand cmd;
    cmd.state = DroneMode::TURNING; 
    cmd.targetVx = 0.0f; 
    cmd.targetVy = 0.0f;
    cmd.angleSpeed = (delta > 0.0) ? ctx.cfg.angularspeed : -ctx.cfg.angularspeed;

    ctx.physics->sendCommand(cmd);

    std::cout << "DEBUG:Turning!" << std::endl;

   
        ctx.direction += (delta > 0.0) ? rotationStep : -rotationStep;
        ctx.direction = DroneContext::normalizeAngle(ctx.direction);
        return nullptr;
    
        
    
}

float StateTurning::estimateTimeToChange(const DroneContext& ctx) {
    Target targetPos = ctx.provider -> getTarget(ctx.currentTargetIdx);
    float targetAngle = std::atan2(targetPos.pos.y - ctx.y, targetPos.pos.x - ctx.x); 
    float diff_angle = DroneContext::normalizeAngle(targetAngle - ctx.direction);

    return (std::abs(diff_angle) / ctx.cfg.angularspeed);
}

bool StateTurning::isFinished() const {
    return false; 
}