#include "../../include/states/StateDeclareting.hpp"
#include "../../include/states/StateTurning.hpp"

std::unique_ptr<IDroneState> StateDeclareting::execute (DroneContext& ctx) {

    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    
    if (ctx.currentSpeed > 0) {
        ctx.currentSpeed -=  acceleration * ctx.cfg.simtimestep;
    
        if (ctx.currentSpeed <= 0) {
        ctx.currentSpeed = 0;
        }

        return nullptr;
    }

    Coord targetPos = ctx.provider->getTargetPosition(ctx.currentTargetIdx, ctx.currentTime);
    ctx.desiredDir = std::atan2(targetPos.y - ctx.y, targetPos.x - ctx.x);
    ctx.targetDir = ctx.desiredDir;
    
    return std::make_unique<StateTurning>();
}

float StateDeclareting::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    return ctx.currentSpeed / acceleration;
}

bool StateDeclareting::isFinished() const {
    return false; 
}