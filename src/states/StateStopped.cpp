#include "../../include/states/StateStopped.hpp"
#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"
#include <iostream>
#include <memory>


std::unique_ptr<IDroneState> StateStopped::execute(DroneContext& ctx) {

    ctx.currentTurnRate = 0.0f;
    ctx.currentAccel = 0.0f;
    float targetAngle = std::atan2(ctx.targets.y - ctx.telemetry.y, ctx.targets.x - ctx.telemetry.x);
    float delta = DroneContext::normalizeAngle(targetAngle - ctx.telemetry.dir);

    if (std::fabs(delta) > ctx.config.turnThreshold) {
        return std::make_unique<StateTurning>();
    }

    return std::make_unique<StateAccelerating>();

}

float StateStopped::estimateTimeToChange(const DroneContext& ctx) {
    return 0.0f;
}

bool StateStopped::isFinished() const {
    return false; 
}