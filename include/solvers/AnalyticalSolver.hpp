#pragma once

#include "../interfaces/IBallisticSolver.hpp"

class AnalyticalSolver : public IBallisticSolver {
    private:
    float calculatingTime(float altitude, float attackSpeed, const AmmoParams& ammo) const;
    float horizontalDist(float t_fall, float v0, float m, float d, float l) const;
    
    public:
    BallisticResult csolve( 
        float atitude, 
        float speed,
        const AmmoParams& ammo) const override;
};