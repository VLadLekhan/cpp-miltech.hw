#include "../interfaces/IDroneState.hpp"

class StateStopped : public IDroneState {
public:
    std::unique_ptr<IDroneState>
        execute(DroneContext& ctx) override;

    const char* name() const override {
    return "Stopped";
    }

    float estimateTimeToChange(const DroneContext& ctx) override;
    bool isFinished() const override;
};