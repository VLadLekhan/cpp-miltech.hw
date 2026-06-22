#include "../../include/providers/JsonTargetProvider.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

#include "../../include/Types.hpp"
#include "../../include/nlohmann/json.hpp"

using json = nlohmann::json;


    JsonTargetProvider::JsonTargetProvider(const std::string& file_path) 
    : json_path(file_path) {
    std::ifstream file(file_path);
    if(!file.is_open()) {
        throw std::runtime_error("Помилка: не вдалося відкрити файл за шляхом: " + file_path);
    }

    if (file_path.empty()) {
    throw std::runtime_error("Знайдено порожній шлях цілі!");
    }
    json data;
    try {
        file >> data;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Помилка: файл JSON має некоректний формат! Деталі: " + std::string(e.what()));
    }

    if (!data.contains("targets") || !data["targets"].is_array()) {
        throw std::runtime_error("Помилка: JSON не містить масиву 'targets'!");
    }

    auto targets_array = data["targets"]; 

    for (const auto& target_item : targets_array) {
        if (target_item.contains("positions") && target_item["positions"].is_array()) {
            std::vector<Coord> path;
            for (const auto& point : target_item["positions"]) {
                float x = point.value("x", 0.0f);
                float y = point.value("y", 0.0f);
                path.push_back({x, y});
            }
            json_targets.push_back(path);
        }
    }

    if (json_targets.empty()) {
        throw std::runtime_error("Помилка: файл JSON порожній або має некоректний формат!");
    }
}

    int JsonTargetProvider::getTargetsCount() {
        return json_targets.size();
    }
    
   Coord JsonTargetProvider::getTargetPosition(int i, float t) {

    if (i < 0 || i >= json_targets.size() || json_targets[i].empty()) {
        std::cerr << "ERROR: Invalid target index: " << i << std::endl;
        return {0.0f, 0.0f}; 
    }
    float timestep = (cfg.arraytimestep > 0.0001f) ? cfg.arraytimestep : 1.0f;

    const auto& path = json_targets[i];

    int idx = (int)std::floor(t / timestep) % path.size();
    int next = (idx + 1) % path.size();
    
    float frac = (t - idx * timestep) / timestep;

    float x = path[idx].x + (path[next].x - path[idx].x) * frac;
    float y = path[idx].y + (path[next].y - path[idx].y) * frac;

    return {x, y};
}
  