#pragma once
#include "../drone_link.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class UartManager {
    private:
    int fd;                 
    dlink::Parser parser;
    dlink::Telemetry lastTelemetry_;
    dlink::TargetPos lastTargetPos_;
    dlink::AmmoCfg lastAmmoCfg_;
    dlink::DroneCfg droneConfig_;

    int init(const char* dev);

    public:
    UartManager(const char* dev);
    ~UartManager() {if (fd >= 0) { close(fd);}}

    void update ();

    dlink::Telemetry getTelemetry() {return lastTelemetry_;}
    dlink::TargetPos getTargetPos(){return lastTargetPos_;} 
    dlink::AmmoCfg getAmmo(){return lastAmmoCfg_;} 
    dlink::DroneCfg getConfig() {return droneConfig_;}

    void sendControl(float accel, float turnRate);
};