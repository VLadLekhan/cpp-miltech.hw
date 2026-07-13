#pragma once

#include <vector>
#include <memory>
#include "../../include/drone_link.hpp"
#include "../../include/DroneContext.hpp"
#include "../../include/interfaces/IDroneState.hpp"
class TargetsProvider{
    private:
    std::vector<dlink::TargetPos> allTargets;
    dlink::AmmoCfg value;
    // DroneContext ctx_;
    //  std::unique_ptr<IDroneState> currentState_;

    public:
    void updateTargets(const dlink::TargetPos& t);
    int selectBestTarget(const DroneContext& ctx, IDroneState& currentState);
    float calculateDistance(float x, float y, dlink::TargetPos c);
    dlink::TargetPos getTarget(int id) const;
};