#include "../../include/states/StateMoving.hpp"
#include "../../include/states/StateTurning.hpp"
#include "../../include/interfaces/ITargetProvider.hpp"
#include "../../include/Types.hpp"


#include <iostream>
#include <cmath>                     



std::unique_ptr<IDroneState> StateMoving::execute(DroneContext& ctx) {

    ctx.currentTurnRate = 0.0f;
    BallisticResult result = ctx.solver->csolve(ctx.telemetry.z, ctx.telemetry.speed, ctx.ammo);

    float tx = ctx.targets.x;
    float ty = ctx.targets.y;

    float distToDrop = std::hypot(tx - ctx.telemetry.x, ty - ctx.telemetry.y);

    if (distToDrop <= ctx.ammo.hitRadius) {
        std::cout << "[LOG] Бомбу скинуто на ціль: " << ctx.currentTargetIdx << std::endl;
        dropped_ = true;
        ctx.missionCompleted = true;
        ctx.dropRequested = true;
        return nullptr;
    }

    std::cout << "DEBUG: Aiming at: (" << tx << ", " << ty << ") | Dist: " << distToDrop << std::endl;
    
    float angleToTarget = std::atan2(ty - ctx.telemetry.y, tx - ctx.telemetry.x);
    float diff = DroneContext::normalizeAngle(angleToTarget - ctx.telemetry.dir);

    if (std::abs(diff) > 0.3f) {
        return std::make_unique<StateTurning>();
    }

    ctx.currentAccel = 0.0f;
    return nullptr;
}

float StateMoving::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.config.attackSpeed, 2.0) / (2.0 * ctx.config.accelerationPath);
    return ctx.config.attackSpeed / acceleration;
}

bool StateMoving::isFinished() const { return dropped_; }

