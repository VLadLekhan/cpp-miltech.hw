#include "../../include/states/StateDeclareting.hpp"
#include "../../include/states/StateTurning.hpp"
#include <iostream>


std::unique_ptr<IDroneState> StateDeclareting::execute (DroneContext& ctx) {

      ctx.currentTurnRate = 0.0f;

   if (ctx.telemetry.speed > 0.0f){
    ctx.currentAccel = -1.0f;
    std::cout << "DEBUG: Declarating, telemetry speed: " << ctx.telemetry.speed << std::endl;
    return nullptr;
   }

   ctx.currentAccel = 0.0f;
   return std::make_unique<StateTurning>();
}


float StateDeclareting::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.config.attackSpeed, 2.0) / (2.0 * ctx.config.accelerationPath);
    if (acceleration < 0.01f) acceleration = 1.0f;
    return (ctx.config.attackSpeed - ctx.telemetry.speed) / acceleration;
}

bool StateDeclareting::isFinished() const {
    return false; 
}
