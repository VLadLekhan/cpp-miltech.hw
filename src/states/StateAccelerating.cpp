#include "../../include/states/StateAccelerating.hpp"
#include "../../include/states/StateMoving.hpp"
#include <iostream>
#include <memory>

std::unique_ptr<IDroneState> StateAccelerating::execute (DroneContext& ctx) {

   ctx.currentTurnRate = 0.0f;

   if (ctx.telemetry.speed < ctx.config.attackSpeed){
    ctx.currentAccel = 1.0f;
    std::cout << "DEBUG: Accelerating, telemetry speed: " << ctx.telemetry.speed << std::endl;
    return nullptr;
   }

   ctx.currentAccel = 0.0f;
   return std::make_unique<StateMoving>();
}


float StateAccelerating::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.config.attackSpeed, 2.0) / (2.0 * ctx.config.accelerationPath);
    if (acceleration < 0.01f) acceleration = 1.0f;
    return (ctx.config.attackSpeed - ctx.telemetry.speed) / acceleration;
}

bool StateAccelerating::isFinished() const {
    return false; 
}