#include "../../include/providers/TargetsProvider.hpp"
#include <iostream>
#include "../../include/states/StateDeclareting.hpp"


void TargetsProvider::updateTargets(const dlink::TargetPos& t) {
    if(t.id < allTargets.size()){
        allTargets[t.id] = t;
    } else {
        allTargets.push_back(t);
    }
}

float TargetsProvider::calculateDistance(float x, float y, dlink::TargetPos c) {
    return std::sqrt(std::pow(x - c.x, 2) + std::pow(y - c.y, 2));
}

dlink::TargetPos TargetsProvider::getTarget(int id) const {
    if (id >= 0 && id < (int)allTargets.size()) return allTargets[id];
    return {};
}

int TargetsProvider::selectBestTarget(const DroneContext& ctx, IDroneState& currentState){

    if (ctx.missionCompleted) return ctx.currentTargetIdx;
    if (!ctx.ammo.nTargets || !ctx.solver) return ctx.currentTargetIdx;

    float minTotalTime = std::numeric_limits<float>::max();
    int bestTarget = -1; 

    for (int i = 0; i < ctx.ammo.nTargets; ++i) {
        if (i >= (int)allTargets.size()) continue;
        const dlink::TargetPos& target = allTargets[i]; 
        float distance = calculateDistance(ctx.telemetry.x, ctx.telemetry.y, target);

        BallisticResult result = ctx.solver->csolve(ctx.telemetry.z, ctx.telemetry.speed, ctx.ammo);

        float flightTime = result.timeOfFlight * (distance / result.horizontalDist);
        float timeToStop = currentState.estimateTimeToChange(ctx);

        float totalTime = flightTime + timeToStop;

        if (i == ctx.currentTargetIdx) totalTime *= 0.95f;

        if (totalTime < minTotalTime) {
            minTotalTime = totalTime;
            bestTarget = i;
         }

    }     

    return bestTarget;
}