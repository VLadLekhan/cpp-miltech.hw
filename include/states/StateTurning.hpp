#include "../interfaces/IDroneState.hpp"

class StateTurning : public IDroneState {
    public:
    std::unique_ptr<IDroneState>
        execute(DroneContext& ctx) override;

     const char* name() const override { return "Turning"; }

     float estimateTimeToChange(const DroneContext& ctx) override;
     bool isFinished() const override;

};