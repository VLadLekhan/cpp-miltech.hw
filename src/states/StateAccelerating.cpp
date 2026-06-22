#include "../../include/states/StateAccelerating.hpp"
#include "../../include/states/StateMoving.hpp"
#include <iostream>

std::unique_ptr<IDroneState> StateAccelerating::execute (DroneContext& ctx) {

    if (ctx.currentSpeed >= ctx.cfg.attackSpeed) {
        ctx.currentSpeed = ctx.cfg.attackSpeed;
        return std::make_unique<StateMoving>();
    }

    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    std::cout << "DEBUG: Current Acceleration: " << acceleration << std::endl;
    ctx.currentSpeed += acceleration * ctx.cfg.simtimestep;
    
        if (ctx.currentSpeed >= ctx.cfg.attackSpeed) {
        ctx.currentSpeed = ctx.cfg.attackSpeed;
        }

        if (acceleration < 0.001f) acceleration = 1.0f;

        return nullptr;
    }

float StateAccelerating::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    return ctx.currentSpeed / acceleration;
}

bool StateAccelerating::isFinished() const {
    return false; 
}