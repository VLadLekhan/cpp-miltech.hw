#include "../../include/solvers/AnalyticalSolver.hpp"

BallisticResult AnalyticalSolver::csolve(float atitude, float speed,const AmmoParams& ammo) const {
   float t = calculatingTime(atitude, speed, ammo);
   float hDist = horizontalDist(t, speed, ammo.mass, ammo.drag, ammo.lift);
   return {t, hDist};
}

float AnalyticalSolver::calculatingTime(float zd,float attackSpeed, const AmmoParams& ammo) const {
    float m = ammo.mass;
    float d = ammo.drag;
    float l = ammo.lift;

    float g = 9.81f;

    float a = (d * g * m) - (2.0 * d * d * l * attackSpeed);
    float b = (-3.0 * g * m * m) + (3.0 * d * l * m * attackSpeed);
    float c = 6.0 * m * m * zd;

    float p = -(b * b) / (3.0 * a * a);
    float q = (2.0 * b * b * b) / (27.0 * a * a * a) + (c / a);

    float arg_arccos = ((3.0 * q) / (2.0 * p)) * std::sqrt(-3.0 / p);

    if (arg_arccos < -1.0) {
        arg_arccos = -1.0;
    }
    else if (arg_arccos > 1.0) {
        arg_arccos = 1.0;
    }

    float phi = std::acos(arg_arccos);

    float t = (2.0f * std::sqrt(-p / 3.0f)) * std::cos((phi + 4.0f * M_PI) / 3.0f) - b / (3.0f * a);

    return (t > 0) ? t : -0.1f;
}

float AnalyticalSolver::horizontalDist(float t, float v0, float m, float d, float l) const {
    float g = 9.81f;

    float t1 = v0 * t;
    float t2 = -(t * t * d * v0) / (2.0f * m);
    float t3 = (t * t * t * (6 * d * g * l * m - 6 * d * d * (l * l - 1) * v0)) / (36.0f * m * m);
    float t4 = (t * t * t * t * (-6 * d * d * g * l * (1 + l * l + l * l * l * l) * m + 3 * d * d * d * l * l * (1 + l * l) * v0 + 6 * d * d * d * l * l * l * l * (1 + l * l) * v0)) / (36.0f * std::pow(1 + l * l, 2) * m * m * m);
    float t5 = (t * t * t * t * t * (3 * d * d * d * g * l * l * l * m - 3 * d * d * d * d * l * l * (1 + l * l) * v0)) / (36.0f * (1 + l * l) * m * m * m * m);

    return t1 + t2 + t3 + t4 + t5;
}

