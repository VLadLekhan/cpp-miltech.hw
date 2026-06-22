#include "../include/MissionProcessor.hpp"
#include <limits>
#include <iostream>

   MissionProcessor::MissionProcessor(std::unique_ptr<IBallisticSolver> solver, std::unique_ptr<IConfigLoader> loader, std::unique_ptr<ITargetProvider> provider) :
   solver_(std::move(solver)), loader_(std::move(loader)), provider_(std::move(provider)){}

   void MissionProcessor::init(std::unique_ptr<IDroneState> droneState) {
    loader_->load();
    ctx_.cfg = loader_->getConfig();
    ctx_.solver = std::move(solver_); 
    ctx_.provider = std::move(provider_);
    currentState_ = std::move(droneState);
    ctx_.missionCompleted = false;
   }

   float MissionProcessor::calculateDistance(float x, float y, Coord c) {
    return std::sqrt(std::pow(x - c.x, 2) + std::pow(y - c.y, 2));
   }

   void MissionProcessor::selectBestTarget() {

      if (ctx_.missionCompleted) return;

    float inertia = 0.95f;
    float minTotalTime = std::numeric_limits<float>::max();
    int bestTarget = -1;

    if (!ctx_.provider || !ctx_.solver) return; 

    for (int i = 0; i < ctx_.provider->getTargetsCount(); ++i) {
        Coord targertPos = ctx_.provider->getTargetPosition(i, ctx_.currentTime);
        float distance = calculateDistance(ctx_.x, ctx_.y, targertPos);

        BallisticResult result = ctx_.solver->csolve(ctx_.cfg.altitude, ctx_.currentSpeed, ctx_.cfg.ammo);

        float flightTime = result.timeOfFlight * (distance / result.horizontalDist);
        float timeToStop = currentState_->estimateTimeToChange(ctx_);

        float totalTime = flightTime + timeToStop;

        float currentTotalTime = (i == ctx_.currentTargetIdx) ? (totalTime * inertia) : totalTime;

        if (i == ctx_.currentTargetIdx) {
            currentTotalTime *= inertia;
        }

        if (currentTotalTime < minTotalTime) {
            minTotalTime = currentTotalTime;
            bestTarget = i;
        }

        
      }
   if (bestTarget != ctx_.currentTargetIdx && bestTarget != -1) {
      std::cout << "[LOG] Перемикання на ціль: " << bestTarget << std::endl;
      ctx_.currentTargetIdx = bestTarget;
         
   }
}
 
   DropPoint MissionProcessor::step() {
      ctx_.currentTime += ctx_.cfg.simtimestep;
      selectBestTarget();

      if (currentState_) {
        auto nextState = currentState_->execute(ctx_);
        if (nextState) {
            currentState_ = std::move(nextState);
            std::cout << "[LOG] Зміна стану: " << currentState_->name() << std::endl;
        }
    }

      Coord targetPos = ctx_.provider->getTargetPosition(ctx_.currentTargetIdx, ctx_.currentTime);

      if (std::abs(targetPos.x) > 1e6 || std::abs(targetPos.y) > 1e6) {
      std::cerr << "АНОМАЛІЯ: Координати цілі вилетіли в космос: " << targetPos.x << ", " << targetPos.y << std::endl;
      }

      float dist = calculateDistance(ctx_.x, ctx_.y, targetPos);
      std::cout << "DEBUG: Час: " << ctx_.currentTime 
          << " | Дистанція до цілі: " << dist << std::endl;

      return {ctx_.cfg.droppoint.fire.x, ctx_.cfg.droppoint.fire.y,};

   }

   void MissionProcessor::changeSolver(std::unique_ptr<IBallisticSolver> s) {
      solver_ = std::move(s);
   }