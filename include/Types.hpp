#pragma once

#include <string>

#include <cmath>

enum class SolverType {
    ANALYTICAL,
    TABLE
};

enum class ProviderType {
    JSON
};

enum class LoaderType {
    FILE
};

struct Coord {
    float x = 0.0f;
    float y = 0.0f;

    Coord operator+(const Coord& other) const { return {x + other.x, y + other.y};}
    Coord operator-(const Coord& other) const { return {x - other.x, y - other.y}; }
    Coord operator*(float s) const { return {x * s, y * s}; }
    Coord operator/(float s) const { return {x / s, y / s}; }

    float lenghth() const {return std::hypot(x,y);}
    Coord normalize() const {
        float len = lenghth();
        return (len > 0) ? Coord{x / len, y / len} : Coord{0,0};
    }
};

struct AmmoParams {
    std::string name;
    float mass;
    float drag;
    float lift;
};

struct DropPoint {
    Coord fire;
    bool isDroped;
};

struct DroneConfig {
    Coord startPos;
    float altitude;
    float initialDir;
    float attackSpeed;
    float accelerationPath;
    std::string ammoName;  
    AmmoParams ammo;   
    float arraytimestep;    
    float targetTimestep;      
    float hitradius;        
    float angularspeed;     
    float turnThreshold;
    DropPoint droppoint;
};

struct SimStep {
    Coord pos;          
    float direction;    
    int   state;        
    int   targetIdx;  
    Coord dropPoint;    
    Coord aimPoint;     
    Coord predictedTarget;
};

enum class DroneMode {
    STOPPED,
    ACCELERATING,
    DECELERATING,
    TURNING,
    MOVING
};

struct DroneTelemetry {
    Coord pos;
    Coord speed;
    float timeSecSinceStart;
};

struct DroneCommand {
	DroneMode state;   // новий режим
	float angleSpeed;  	// Кутова швидкість повороту
};





