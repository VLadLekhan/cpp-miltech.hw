#include "../../include/states/StateStopped.hpp"
#include <memory>
#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"

std::unique_ptr<IDroneState> StateStopped::execute(DroneContext& ctx) {
    Coord targetPos = ctx.provider->getTargetPosition(ctx.currentTargetIdx, ctx.currentTime);
    ctx.desiredDir = std::atan2(targetPos.y - ctx.y, targetPos.x - ctx.x);

    float delta = DroneContext::normalizeAngle(
            ctx.desiredDir - ctx.direction);
 
        if (std::fabs(delta) > ctx.cfg.turnThreshold) {
            ctx.targetDir = ctx.desiredDir;
        return std::make_unique<StateTurning>();
        }
        
        ctx.direction = ctx.desiredDir;
        return std::make_unique<StateAccelerating>();

}

float StateStopped::estimateTimeToChange(const DroneContext& ctx) {
    return 0.0f;
}

bool StateStopped::isFinished() const {
    return false; 
}