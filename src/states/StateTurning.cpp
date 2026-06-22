#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"

std::unique_ptr<IDroneState> StateTurning::execute(DroneContext& ctx) {
    Coord targetPos = ctx.provider->getTargetPosition(ctx.currentTargetIdx, ctx.currentTime);
    ctx.desiredDir = std::atan2(targetPos.y - ctx.y, targetPos.x - ctx.x);

    float delta = DroneContext::normalizeAngle(ctx.desiredDir - ctx.direction);
    float rotationStep = ctx.cfg.simtimestep * ctx.cfg.angularspeed;

    if (std::abs(delta) > 0.01f) { 
        ctx.direction += (delta > 0.0) ? rotationStep : -rotationStep;
        ctx.direction = DroneContext::normalizeAngle(ctx.direction);
        return nullptr;
    } else {
        ctx.direction = ctx.desiredDir;
        return std::make_unique<StateAccelerating>();
    }
}

float StateTurning::estimateTimeToChange(const DroneContext& ctx) {
    Coord targetPos = ctx.provider -> getTargetPosition(ctx.currentTargetIdx, ctx.currentTime);
    float targetAngle = std::atan2(targetPos.y - ctx.y, targetPos.x - ctx.x); 
    float diff_angle = DroneContext::normalizeAngle(targetAngle - ctx.direction);

    return (std::abs(diff_angle) / ctx.cfg.angularspeed);
}

bool StateTurning::isFinished() const {
    return false; 
}