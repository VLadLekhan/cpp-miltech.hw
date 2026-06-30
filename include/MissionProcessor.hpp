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

    class MissionProcessor {
        private:
        DroneContext ctx_; 
        std::unique_ptr<IDroneState> currentState_;

        std::unique_ptr<IBallisticSolver> solver_;
        std::unique_ptr<IConfigLoader> loader_;
        std::shared_ptr<ITargetProvider> provider_;
        std::shared_ptr<DronePhysics> physics_;

        std::atomic<bool> isRunning{false};

        public:
        MissionProcessor(std::unique_ptr<IBallisticSolver> solver, std::unique_ptr<IConfigLoader> loader, std::shared_ptr<ITargetProvider> provider, std::shared_ptr<DronePhysics> physics);
        
        void init(std::unique_ptr<IDroneState> droneState);
        float calculateDistance(float x, float y, Target c);
        void selectBestTarget();
        void run();
        void stop();
        DropPoint step();
        IDroneState* getCurrentState() const { return currentState_.get(); }
        DroneContext& getContext() { return ctx_; }

        void reset();
        void changeSolver(std::unique_ptr<IBallisticSolver> s);

    };
