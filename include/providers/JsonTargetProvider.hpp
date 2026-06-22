#pragma once 
#include <vector>
#include <string>

#include "../interfaces/ITargetProvider.hpp"




    class JsonTargetProvider : public ITargetProvider {
         private: 
         std::vector<std::vector<Coord>> json_targets;
         std::string json_path;
         DroneConfig cfg;

         public: 
         JsonTargetProvider (const std::string& filePath);
         int getTargetsCount() override;
         Coord getTargetPosition(int i, float t) override;
         ~JsonTargetProvider() override {} ;

    };
