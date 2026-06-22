#include "../interfaces/IDroneState.hpp"

class StateAccelerating : public IDroneState {
    public:
    std::unique_ptr<IDroneState> 
        execute (DroneContext& ctx) override;

    const char* name() const override{
        return "Accelereting";
    }

    float estimateTimeToChange(const DroneContext& ctx) override;
    bool isFinished() const override;
};