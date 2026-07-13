#pragma once 
#include <vector>
#include <string>
#include<thread>
#include<atomic>
#include<mutex>

#include "../interfaces/ITargetProvider.hpp"





    class JsonTargetProvider : public ITargetProvider {
         private: 
         std::vector<Target> targetPos_;
         float targetTimeFromStart;
         std::vector<std::vector<Target>> json_targets;
         std::string json_path;
         DroneConfig cfg;

         std::thread workerThread;
         std::atomic<bool> isRunning;
         std::atomic<bool> threadReady_{false};
         mutable std::mutex mtx;
        
         //ThreadSafeQueue<DroneCommand> commandQueue;
         
         Target calculateNextStateInternal(int i, float t);
         Target calculateNextState(int i, float t);
         
         public: 
         JsonTargetProvider (const std::string& filePath);
         int getTargetsCount() override;
         Target getTarget(int i) override;
         void run();
         void setConfig(const DroneConfig& config);
         bool isThreadReady() override;
         void start() override;
         void stop() override;

         ~JsonTargetProvider() override {} ;

    };
