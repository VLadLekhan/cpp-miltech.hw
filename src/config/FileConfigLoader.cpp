#include "../../include/config/FileConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include "../../include/nlohmann/json.hpp"

using json = nlohmann::json;

FileConfigLoader::FileConfigLoader(const std::string& filepath) : filePath_(filepath) {}

void FileConfigLoader::load() {
    std::ifstream cfgFile(filePath_);
    if (!cfgFile.is_open()) throw std::runtime_error("Не вдалося відкрити конфіг");
    
    json data;
    cfgFile >> data;

    m_config.startPos.x = data["drone"]["position"]["x"].get<float>();
    m_config.startPos.y = data["drone"]["position"]["y"].get<float>();
    m_config.altitude = data["drone"]["altitude"].get<float>();
    m_config.initialDir = data["drone"]["initialDirection"].get<float>();
    m_config.attackSpeed = data["drone"]["attackSpeed"].get<float>();
    m_config.accelerationPath = data["drone"]["accelerationPath"].get<float>();
    m_config.angularspeed = data["drone"]["angularSpeed"].get<float>();
    m_config.turnThreshold = data["drone"]["turnThreshold"].get<float>();
    m_config.hitradius = data["simulation"]["hitRadius"].get<float>();
    m_config.arraytimestep = data["targetArrayTimeStep"].get<float>();
    m_config.simtimestep = data["simulation"]["timeStep"].get<float>();
    m_config.ammo = m_ammo;
    
    std::string ammoName = data.at("ammo").get<std::string>();

    std::ifstream ammoFile("data/ammo.json");
    if (!ammoFile.is_open()) throw std::runtime_error("Не вдалося відкрити data/ammo.json");
    
    json ammoList;
    ammoFile >> ammoList;

    if (!ammoList.is_array()) {
        throw std::runtime_error("ammo.json має бути масивом (починатися з [ )");
    }

    bool found = false;
    for (const auto& item : ammoList) {
        if (item.at("name").get<std::string>() == ammoName) {
            m_ammo.mass = item.at("mass").get<float>();
            m_ammo.drag = item.at("drag").get<float>();
            m_ammo.lift = item.at("lift").get<float>();
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Warning: Ammo " << ammoName << " not found!" << std::endl;
    }
}

DroneConfig FileConfigLoader::getConfig() { return m_config; }
AmmoParams FileConfigLoader::getAmmoParams() { return m_ammo; }