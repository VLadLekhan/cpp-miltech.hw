#include "../include/MissionProcessor.hpp"
#include "../include/states/StateDeclareting.hpp"
#include <algorithm>
#include <limits>
#include <iostream>

   MissionProcessor::MissionProcessor(std::unique_ptr<IBallisticSolver> solver, std::unique_ptr<IConfigLoader> loader, std::shared_ptr<ITargetProvider> provider, std::shared_ptr<DronePhysics> physics) :
   solver_(std::move(solver)), loader_(std::move(loader)), provider_(std::move(provider)), physics_(std::move(physics)){}

   void MissionProcessor::init(std::unique_ptr<IDroneState> droneState) {
    loader_->load();
    ctx_.cfg = loader_->getConfig();
    ctx_.solver = std::move(solver_); 
    ctx_.provider = provider_.get();
    ctx_.physics = physics_.get();
    isRunning = true;
    currentState_ = std::move(droneState);
    ctx_.missionCompleted = false;
    ctx_.currentTargetIdx = 0;
    ctx_.lastProcessedTargetIdx = -1;
    ctx_.currentTime = 0.0f;
   }

   float MissionProcessor::calculateDistance(float x, float y, Target c) {
    return std::sqrt(std::pow(x - c.pos.x, 2) + std::pow(y - c.pos.y, 2));
   }

   void MissionProcessor::selectBestTarget() {

      if (ctx_.missionCompleted) return;

    float inertia = 0.95f;
    float minTotalTime = std::numeric_limits<float>::max();
    int bestTarget = -1;

    if (!ctx_.provider || !ctx_.solver) return; 

    for (int i = 0; i < ctx_.provider->getTargetsCount(); ++i) {
        Target targertPos = ctx_.provider->getTarget(i);
        float distance = calculateDistance(ctx_.x, ctx_.y, targertPos);

        BallisticResult result = ctx_.solver->csolve(ctx_.cfg.altitude, ctx_.currentSpeed, ctx_.cfg.ammo);

        float flightTime = result.timeOfFlight * (distance / result.horizontalDist);
        float timeToStop = currentState_->estimateTimeToChange(ctx_);

        float totalTime = flightTime + timeToStop;

        if (i == ctx_.currentTargetIdx) totalTime *= 0.95f;

        if (totalTime < minTotalTime) {
            minTotalTime = totalTime;
            bestTarget = i;
         }

      }
         if (bestTarget != ctx_.currentTargetIdx && bestTarget != -1) {
      std::cout << "[LOG] Перемикання на ціль: " << bestTarget << std::endl;
      ctx_.currentTargetIdx = bestTarget;

      currentState_ = std::make_unique<StateDeclareting>();
      }     
   
} 
   DropPoint MissionProcessor::step() {
     DroneTelemetry droneTelemetry = ctx_.physics->getTelemetry();

     ctx_.x = droneTelemetry.pos.x;
     ctx_.y = droneTelemetry.pos.y;
     ctx_.currentSpeed = std::sqrt(std::pow(droneTelemetry.speed.x, 2) + std::pow(droneTelemetry.speed.y, 2));
     ctx_.currentTime = droneTelemetry.timeSecSinceStart;

     selectBestTarget();

     if(currentState_){
      auto nextState = currentState_->execute(ctx_);
      if(nextState){
         std::cout << "[LOG] Зміна стану на: " << nextState->name() << std::endl;
         currentState_ = std::move(nextState);
      }
     }
     return {ctx_.cfg.droppoint.fire.x, ctx_.cfg.droppoint.fire.y};
   }

   void MissionProcessor::run() {
    // Розрахунок періоду виконання на основі конфігу
    auto sleepDuration = std::chrono::milliseconds(static_cast<int>(ctx_.cfg.physicsTimeStep * 1000));
    
    while (isRunning) {
        step();
        std::this_thread::sleep_for(sleepDuration);
    }
}

void MissionProcessor::stop() {
    isRunning = false;
}

   void MissionProcessor::changeSolver(std::unique_ptr<IBallisticSolver> s) {
      solver_ = std::move(s);
   }