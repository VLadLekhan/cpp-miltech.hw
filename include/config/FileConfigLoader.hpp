#pragma once

#include <string>

#include "../interfaces/IConfigLoader.hpp"



class FileConfigLoader : public IConfigLoader {
    private:
    DroneConfig m_config;
    AmmoParams m_ammo;
    std::string filePath_;

    public:
    FileConfigLoader(const std::string& filepath);

    void load() override;
    DroneConfig getConfig() override;
    AmmoParams getAmmoParams() override;
        
    ~FileConfigLoader() {};
};


 