#pragma once 

#include <memory>

#include <cmath>
#include "Types.hpp"
#include "DroneContext.hpp" 
#include "interfaces/IDroneState.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/ITargetProvider.hpp"
#include "interfaces/IConfigLoader.hpp"
#include "../include/DronePhysics.hpp"
#include "../include/drivers/uart_manager.hpp"
#include "../include/providers/TargetsProvider.hpp"
#include "../include/drivers/gpiod.hpp"

    class MissionProcessor {
        private:
        DroneContext ctx_; 
        UartManager& uart_;
        GpioManager& gpio_;
        std::unique_ptr<IDroneState> currentState_;
        TargetsProvider target_;
        

        std::unique_ptr<IBallisticSolver> solver_;
        std::unique_ptr<IConfigLoader> loader_;
        std::shared_ptr<ITargetProvider> provider_;
        std::shared_ptr<DronePhysics> physics_;

        std::atomic<bool> isRunning{false};

        public:
        MissionProcessor(std::unique_ptr<IBallisticSolver> solver, UartManager* uart, GpioManager* gpio);
        
        void init(std::unique_ptr<IDroneState> droneState);
        float calculateDistance(float x, float y, dlink::TargetPos c);
        void selectBestTarget();
        void run();
        void stop();
        dlink::Control step();
        IDroneState* getCurrentState() const { return currentState_.get(); }
        DroneContext& getContext() { return ctx_; }

        void reset();
        void changeSolver(std::unique_ptr<IBallisticSolver> s);

    };
