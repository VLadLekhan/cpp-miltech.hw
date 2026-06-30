#include "../../include/states/StateAccelerating.hpp"
#include "../../include/states/StateMoving.hpp"
#include <iostream>

std::unique_ptr<IDroneState> StateAccelerating::execute (DroneContext& ctx) {

   

    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    if (acceleration < 0.001f) acceleration = 1.0f;

    float nextSpeed = ctx.currentSpeed + (acceleration * ctx.cfg.physicsTimeStep);

    if (nextSpeed >= ctx.cfg.attackSpeed) {
        nextSpeed = ctx.cfg.attackSpeed;
    }

    DroneCommand cmd;
    cmd.state = DroneMode::ACCELERATING; 
    cmd.targetVx = nextSpeed * std::cos(ctx.direction);
    cmd.targetVy = nextSpeed * std::sin(ctx.direction);
    cmd.angleSpeed = 0.0f;

    ctx.physics->sendCommand(cmd);

    ctx.currentSpeed = nextSpeed;

    std::cout << "DEBUG: Accelerating, current speed: " << ctx.currentSpeed << std::endl;

     if (ctx.currentSpeed >= ctx.cfg.attackSpeed) {
        return std::make_unique<StateMoving>();
    }

    return nullptr;
}

float StateAccelerating::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    return ctx.currentSpeed / acceleration;
}

bool StateAccelerating::isFinished() const {
    return false; 
}