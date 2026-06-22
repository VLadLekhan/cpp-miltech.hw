#include "../interfaces/IDroneState.hpp"

class StateMoving : public IDroneState {
    private: 
    bool dropped_ = false;

    public:
    std::unique_ptr<IDroneState> 
        execute (DroneContext& ctx) override;

    const char* name() const override{
        return "Moving";
    }

    float estimateTimeToChange(const DroneContext& ctx) override;

    bool isFinished() const override ;
};