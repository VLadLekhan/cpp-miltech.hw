#pragma once 

#include <memory>

#include <cmath>
#include "Types.hpp"
#include "DroneContext.hpp" 
#include "interfaces/IDroneState.hpp"
#include "interfaces/IBallisticSolver.hpp"
#include "interfaces/ITargetProvider.hpp"
#include "interfaces/IConfigLoader.hpp"


    class MissionProcessor {
        private:
        DroneContext ctx_; 
        std::unique_ptr<IDroneState> currentState_;

        std::unique_ptr<IBallisticSolver> solver_;
        std::unique_ptr<IConfigLoader> loader_;
        std::unique_ptr<ITargetProvider> provider_;

        public:
        MissionProcessor(std::unique_ptr<IBallisticSolver> solver, std::unique_ptr<IConfigLoader> loader, std::unique_ptr<ITargetProvider> provider);
        
        void init(std::unique_ptr<IDroneState> droneState);
        float calculateDistance(float x, float y, Coord c);
        void selectBestTarget();
        DropPoint step();
        IDroneState* getCurrentState() const { return currentState_.get(); }
        DroneContext& getContext() { return ctx_; }

        void reset();
        void changeSolver(std::unique_ptr<IBallisticSolver> s);

    };
