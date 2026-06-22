#pragma once 

#include <vector>
#include "../interfaces/IBallisticSolver.hpp"

struct BallisticTable{
    std::vector<float> axisZ0, axisV0, axisM, axisD, axisL;
    struct Result {float t; float hDist;};
    std::vector<Result> data;

    size_t index(int iz, int iv, int im, int id, int il) const {
        return ((((size_t)iz * axisV0.size() + iv)
                              * axisM.size()  + im)
                              * axisD.size()  + id)
                              * axisL.size()  + il;
    }

    const Result& at(int iz, int iv, int im,
                     int id, int il) const {
        return data[index(iz, iv, im, id, il)];
    }   
    
    bool load(const char* path);

    //Result lerp(const Result& a, const Result& b, float t);

    struct Interp {
    int lo;      // нижній індекс в осі
    float frac;  // коефіцієнт [0..1]
    };

    Interp findInterp(float val, const std::vector<float>& axis) const;

    Result lookup(
    float Z0, float V0, float m,
    float d,  float l) const;
};

class TableSolver : public IBallisticSolver {
    private:
    BallisticTable table_;

    public:
    explicit TableSolver(const std::string& path);
    BallisticResult csolve(
        float atitude, 
        float speed,
        const AmmoParams& ammo) const override;

        
};