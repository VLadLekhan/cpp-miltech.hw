#pragma once

#include <memory>
#include <string>

#include "../Types.hpp"
#include "../interfaces/IBallisticSolver.hpp"
#include "../interfaces/IConfigLoader.hpp"
#include "../interfaces/ITargetProvider.hpp"



    class ComponentFactory {
        public:
        static std::unique_ptr<IBallisticSolver> createSolver(SolverType type);
        static std::unique_ptr<IConfigLoader> createLoader(LoaderType type);
        static std::unique_ptr<ITargetProvider> createProvider(ProviderType type, std::string filepath);
    };
