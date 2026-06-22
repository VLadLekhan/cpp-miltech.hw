#pragma once

#include "../Types.hpp"

struct BallisticResult {
    float timeOfFlight;
    float horizontalDist;
};

class IBallisticSolver{
    public:
    virtual BallisticResult csolve( 
        float atitude, 
        float speed,
        const AmmoParams& ammo) const = 0;
        
    virtual ~IBallisticSolver() {}
};
