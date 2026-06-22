#pragma once
#include <vector>
#include "../Types.hpp"



    class ITargetProvider {
        public:
        virtual int getTargetsCount() = 0;
        virtual Coord getTargetPosition(int i, float t) = 0;
        virtual ~ITargetProvider() {};
    };