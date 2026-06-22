#include "../interfaces/IDroneState.hpp"

class StateDeclareting : public IDroneState {
    public:
    std::unique_ptr<IDroneState> 
        execute (DroneContext& ctx) override;

    const char* name() const override{
        return "Declareting";
    }

    float estimateTimeToChange(const DroneContext& ctx) override;
    bool isFinished() const override;
};
