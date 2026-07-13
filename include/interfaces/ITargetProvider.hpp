#pragma once
#include <vector>
#include "../Types.hpp"

    struct Target {
        Coord pos;
        Coord velocity;
    };


    class ITargetProvider {
        public:
        virtual int getTargetsCount() = 0;
        virtual Target getTarget(int) = 0;
        virtual bool isThreadReady() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual ~ITargetProvider() {};
    };