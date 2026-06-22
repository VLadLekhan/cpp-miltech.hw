#include "../../include/config/ComponentFactory.hpp"
#include "../../include/config/FileConfigLoader.hpp"
#include "../../include/providers/JsonTargetProvider.hpp"
#include "../../include/solvers/AnalyticalSolver.hpp"
#include "../../include/solvers/TableSolver.hpp"


#include <stdexcept>
#include <memory>
#include <string>


    std::unique_ptr<IBallisticSolver> ComponentFactory::createSolver(SolverType type){
        switch (type) {
            case SolverType ::ANALYTICAL:
              return std::make_unique<AnalyticalSolver>();
            case SolverType::TABLE:
                return std::make_unique<TableSolver>("data/ballistic_table.txt");
            default:
             throw std::runtime_error("ComponentFactory Error: Unsupported solver type!");
        }
    }

    std::unique_ptr<IConfigLoader> createLoade(LoaderType type){
        switch (type) {
            case LoaderType::FILE:
             return std::make_unique<FileConfigLoader>("data/config.json");
            default:
             throw std::runtime_error("ComponentFactory Error: Unsupported loader type!");
        }
    }

    std::unique_ptr<ITargetProvider> createProvider(ProviderType type, std::string filepath){
        switch(type){
            case ProviderType::JSON:
            if(filepath.empty()){
                throw std::runtime_error("ComponentFactory Error: JSON provider requires file path params!");
            }
            return std::make_unique<JsonTargetProvider>(filepath);
            default:
            throw std::runtime_error("ComponentFactory Error: Unsupported provider type!");
        }
    }
