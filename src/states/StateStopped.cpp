#include "../../include/states/StateStopped.hpp"
#include "../../include/states/StateTurning.hpp"
#include "../../include/states/StateAccelerating.hpp"
#include <iostream>
#include <memory>


std::unique_ptr<IDroneState> StateStopped::execute(DroneContext& ctx) {

    Target targetPos = ctx.provider->getTarget(ctx.currentTargetIdx);
    ctx.desiredDir = std::atan2(targetPos.pos.y - ctx.y, targetPos.pos.x - ctx.x);
    float delta = DroneContext::normalizeAngle(ctx.desiredDir - ctx.direction);

    DroneCommand cmd;
    cmd.state = DroneMode::STOPPED; 
    cmd.targetVx = 0.0f;
    cmd.targetVy = 0.0f;
    cmd.angleSpeed = 0.0f;
    ctx.physics->sendCommand(cmd);

    std::cout << "DEBUG:Stopped!" << std::endl;

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