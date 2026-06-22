#include "../../include/solvers/TableSolver.hpp"
#include <stdexcept>
#include <fstream>
#include <algorithm>

// Допоміжна функція lerp залишається static, бо вона не є частиною класу
static BallisticTable::Result lerp(const BallisticTable::Result& a, const BallisticTable::Result& b, float t) {
    return {
        a.t     + (b.t     - a.t)     * t,
        a.hDist + (b.hDist - a.hDist) * t
    };
}

// ВИПРАВЛЕНО: Додано префікс BallisticTable:: та const в кінці
BallisticTable::Interp BallisticTable::findInterp(float val, const std::vector<float>& axis) const {
    if (val <= axis.front()) return {0, 0.0f};
    if (val >= axis.back())
        return {(int)axis.size()-2, 1.0f};
 
    auto it = std::lower_bound(axis.begin(), axis.end(), val);
    int i = (int)(it - axis.begin()) - 1;
    if (i < 0) i = 0;
 
    float frac = (val - axis[i]) / (axis[i+1] - axis[i]);
    return {i, frac};
}

bool BallisticTable::load(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    // 1. Зчитуємо розмірності осей (перші 5 чисел)
    size_t sz, sv, sm, sd, sl;
    if (!(file >> sz >> sv >> sm >> sd >> sl)) return false;

    // 2. Зчитуємо дані осей
    axisZ0.resize(sz); for(auto& v : axisZ0) file >> v;
    axisV0.resize(sv); for(auto& v : axisV0) file >> v;
    axisM.resize(sm);  for(auto& v : axisM) file >> v;
    axisD.resize(sd);  for(auto& v : axisD) file >> v;
    axisL.resize(sl);  for(auto& v : axisL) file >> v;

    // 3. Зчитуємо табличні дані (пари t та hDist)
    data.resize(sz * sv * sm * sd * sl);
    for(auto& res : data) {
        if (!(file >> res.t >> res.hDist)) return false;
    }

    return true;
}

BallisticTable::Result BallisticTable::lookup(float Z0, float V0, float m, float d, float l) const {
    Interp iz = findInterp(Z0, axisZ0);
    Interp iv = findInterp(V0, axisV0);
    Interp im = findInterp(m,  axisM);
    Interp id = findInterp(d,  axisD);
    Interp il = findInterp(l,  axisL);
 
    Result v[16];
    for (int a = 0; a < 2; a++)
     for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
       for (int e = 0; e < 2; e++) {
           auto& lo = at(iz.lo+a, iv.lo+b, im.lo+c, id.lo+e, il.lo);
           auto& hi = at(iz.lo+a, iv.lo+b, im.lo+c, id.lo+e, il.lo+1);
           v[a*8+b*4+c*2+e] = lerp(lo, hi, il.frac);
       }
 
    Result w[8];
    for (int a = 0; a < 2; a++)
     for (int b = 0; b < 2; b++)
      for (int c = 0; c < 2; c++)
       w[a*4+b*2+c] = lerp(v[a*8+b*4+c*2], v[a*8+b*4+c*2+1], id.frac);
 
    Result u[4];
    for (int a = 0; a < 2; a++)
     for (int b = 0; b < 2; b++)
      u[a*2+b] = lerp(w[a*4+b*2], w[a*4+b*2+1], im.frac);
 
    Result s[2];
    for (int a = 0; a < 2; a++)
        s[a] = lerp(u[a*2], u[a*2+1], iv.frac);
 
    return lerp(s[0], s[1], iz.frac);
}

TableSolver::TableSolver(const std::string& path) {
    if(!table_.load(path.c_str()))
        throw std::runtime_error("Table load failed");
}

BallisticResult TableSolver::csolve(float atitude, float speed, const AmmoParams& ammo) const {
    auto res = table_.lookup(atitude, speed, ammo.mass, ammo.drag, ammo.lift);
    return {res.t, res.hDist};
}