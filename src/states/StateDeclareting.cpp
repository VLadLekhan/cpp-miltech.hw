#include "../../include/states/StateDeclareting.hpp"
#include "../../include/states/StateTurning.hpp"
#include <iostream>


std::unique_ptr<IDroneState> StateDeclareting::execute (DroneContext& ctx) {

    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    float nextSpeed = std::max(0.0f, ctx.currentSpeed - (acceleration * ctx.cfg.physicsTimeStep));
    
   DroneCommand cmd;
    cmd.state = DroneMode::DECELERATING; // Тепер ми чітко кажемо Фізиці: це гальмування
    cmd.targetVx = nextSpeed * std::cos(ctx.direction);
    cmd.targetVy = nextSpeed * std::sin(ctx.direction);
    cmd.angleSpeed = 0.0f;

    ctx.physics->sendCommand(cmd);
    
    ctx.currentSpeed = nextSpeed;

    std::cout << "DEBUG: Declareting, current speed: " << ctx.currentSpeed << std::endl;

    // 3. Умова завершення гальмування
    if (ctx.currentSpeed <= 0.1f) {
        ctx.currentSpeed = 0.0f;

        Target targetPos = ctx.provider->getTarget(ctx.currentTargetIdx);
        ctx.desiredDir = std::atan2(targetPos.pos.y - ctx.y, targetPos.pos.x - ctx.x);
        ctx.targetDir = ctx.desiredDir;

    return std::make_unique<StateTurning>();
    }
return nullptr;
}

float StateDeclareting::estimateTimeToChange(const DroneContext& ctx) {
    float acceleration = std::pow(ctx.cfg.attackSpeed, 2.0) / (2.0 * ctx.cfg.accelerationPath);
    return ctx.currentSpeed / acceleration;
}

bool StateDeclareting::isFinished() const {
    return false; 
}