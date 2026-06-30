#include "../../include/states/StateMoving.hpp"
#include "../../include/states/StateTurning.hpp"
#include "../../include/interfaces/ITargetProvider.hpp"
#include "../../include/Types.hpp"

#include <iostream>
#include <cmath>                     



std::unique_ptr<IDroneState> StateMoving::execute(DroneContext& ctx) {

    if (!ctx.solver) {
        std::cerr << "CRITICAL ERROR: ctx.solver is NULL in StateMoving!" << std::endl;
        return nullptr;
    }
    if (!ctx.provider) {
        std::cerr << "CRITICAL ERROR: ctx.provider is NULL in StateMoving!" << std::endl;
        return nullptr;
    }

    BallisticResult result = ctx.solver->csolve(ctx.cfg.altitude, ctx.currentSpeed, ctx.cfg.ammo);
    Target targetPos = ctx.provider->getTarget(ctx.currentTargetIdx);
    //Target nextTargetPos = ctx.provider->getTarget(ctx.currentTargetIdx);
    
    
    float vx = targetPos.velocity.x;
    float vy = targetPos.velocity.y;
    float tx = targetPos.pos.x + vx * result.timeOfFlight;
    float ty = targetPos.pos.y + vy * result.timeOfFlight;
    

    float distToDrop = std::hypot(tx - ctx.x, ty - ctx.y);

    if (distToDrop <= ctx.cfg.hitradius) {
        std::cout << "[LOG] Бомбу скинуто на ціль: " << ctx.currentTargetIdx << std::endl;
        dropped_ = true;
        ctx.missionCompleted = true;
        return nullptr;
    }

    std::cout << "DEBUG: Aiming at: (" << tx << ", " << ty << ") | Dist: " << distToDrop << std::endl;
    
    float angleToTarget = std::atan2(ty - ctx.y, tx - ctx.x);
    float diff = DroneContext::normalizeAngle(angleToTarget - ctx.direction);

    if (std::abs(diff) > 0.3f) {
        ctx.desiredDir = angleToTarget;
        return std::make_unique<StateTurning>();
    }

    float speed = std::min(ctx.currentSpeed, 500.0f);

    DroneCommand cmd;
    cmd.state = DroneMode::MOVING;
    cmd.targetVx = speed * std::cos(ctx.direction);
    cmd.targetVy = speed * std::sin(ctx.direction);
    cmd.angleSpeed = 0.0f;

    ctx.physics->sendCommand(cmd);

    ctx.cfg.droppoint.fire.x = ctx.x + result.horizontalDist * std::cos(ctx.direction);
    ctx.cfg.droppoint.fire.y = ctx.y + result.horizontalDist * std::sin(ctx.direction);

    return nullptr;
}

float StateMoving::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    return ctx.cfg.attackSpeed / acceleration;
}

bool StateMoving::isFinished() const { return dropped_; }

