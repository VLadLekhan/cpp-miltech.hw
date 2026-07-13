#pragma once

#include "../drone_link.hpp"

struct BallisticResult {
    float timeOfFlight;
    float horizontalDist;
};

class IBallisticSolver{
    public:
    virtual BallisticResult csolve( 
        float atitude, 
        float speed,
        const dlink::AmmoCfg& ammo) const = 0;
        
    virtual ~IBallisticSolver() {}
};
