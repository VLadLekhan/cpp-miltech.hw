#pragma once

#include "../Types.hpp"


    struct DronePosition {
       float x = 0.0f;
       float y = 0.0f;
    };

    class IConfigLoader{
        public:
        virtual void load() = 0;
        virtual DroneConfig getConfig() = 0;
        virtual AmmoParams getAmmoParams() = 0;
        virtual ~IConfigLoader() {};
    };
