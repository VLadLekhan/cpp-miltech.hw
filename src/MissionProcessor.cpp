#include "../include/MissionProcessor.hpp"
#include "../include/states/StateDeclareting.hpp"
#include <iostream>

   MissionProcessor::MissionProcessor(std::unique_ptr<IBallisticSolver> solver, UartManager* uart, GpioManager* gpio) :
   solver_(std::move(solver)), uart_(*uart), gpio_(*gpio){}

   void MissionProcessor::init(std::unique_ptr<IDroneState> droneState) {
    uart_.update();

     ctx_.telemetry = uart_.getTelemetry();
     ctx_.targets = uart_.getTargetPos();
     ctx_.ammo = uart_.getAmmo();
     ctx_.config = uart_.getConfig();
     ctx_.solver = std::move(solver_);
     
    isRunning = true;
    currentState_ = std::move(droneState);
    ctx_.missionCompleted = false;
    ctx_.currentTargetIdx = 0;
    ctx_.lastProcessedTargetIdx = -1;
   }

   // float MissionProcessor::calculateDistance(float x, float y, dlink::TargetPos c) {
   //  return std::sqrt(std::pow(x - c.x, 2) + std::pow(y - c.y, 2));
   // }

   // void MissionProcessor::selectBestTarget() {

   //    if (ctx_.missionCompleted) return;

   //  float inertia = 0.95f;
   //  float minTotalTime = std::numeric_limits<float>::max();
   //  int bestTarget = -1;

   //  if (!ctx_.ammo.nTargets || !ctx_.solver) return; 

   //  for (int i = 0; i < ctx_.ammo.nTargets; ++i) {
   //      dlink::TargetPos targert = ctx_.provider->getTarget(i);
   //      float distance = calculateDistance(ctx_.telemetry.x, ctx_.telemetry.y, targertPos);

   //      BallisticResult result = ctx_.solver->csolve(ctx_.telemetry.z, ctx_.telemetry.speed, ctx_.ammo);

   //      float flightTime = result.timeOfFlight * (distance / result.horizontalDist);
   //      float timeToStop = currentState_->estimateTimeToChange(ctx_);

   //      float totalTime = flightTime + timeToStop;

   //      if (i == ctx_.currentTargetIdx) totalTime *= 0.95f;

   //      if (totalTime < minTotalTime) {
   //          minTotalTime = totalTime;
   //          bestTarget = i;
   //       }

   //    }
   //       if (bestTarget != ctx_.currentTargetIdx && bestTarget != -1) {
   //    std::cout << "[LOG] Перемикання на ціль: " << bestTarget << std::endl;
   //    ctx_.currentTargetIdx = bestTarget;

   //    currentState_ = std::make_unique<StateDeclareting>();
   //    }     
   
// } 
   dlink::Control MissionProcessor::step() {
     uart_.update();

     ctx_.telemetry = uart_.getTelemetry();
     ctx_.targets = uart_.getTargetPos();
     ctx_.ammo = uart_.getAmmo();

     target_.updateTargets(ctx_.targets);

     int best = target_.selectBestTarget(ctx_, *currentState_);

     ctx_.targets = target_.getTarget(ctx_.currentTargetIdx);
     
     if (best != -1 && best != ctx_.currentTargetIdx) {
         std::cout << "[LOG] Перемикання на ціль: " << best << std::endl;
         ctx_.currentTargetIdx = best;
         currentState_ = std::make_unique<StateDeclareting>();
      }

     if(currentState_){
      auto nextState = currentState_->execute(ctx_);
      
      if (ctx_.dropRequested) {
         gpio_.setDrop();       
         ctx_.dropRequested = false;
      }

      if(nextState){
         std::cout << "[LOG] Зміна стану на: " << nextState->name() << std::endl;
         currentState_ = std::move(nextState);
      }

     }

     uart_.sendControl(ctx_.currentAccel, ctx_.currentTurnRate);

     return {ctx_.currentAccel, ctx_.currentTurnRate};
   }

   void MissionProcessor::run() {
    // Розрахунок періоду виконання на основі конфігу
    auto sleepDuration = std::chrono::milliseconds(static_cast<int>(ctx_.config.timeStep * 1000));
    
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