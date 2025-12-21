#pragma once
#include <string>
#include <mutex>
#include "json.hpp"

using json = nlohmann::json;

struct CameraConfig {
    int id;
    int width;
    int height;
    int fps;
};

struct FaceDetectionConfig {
    bool enabled;
    std::string protoPath;
    std::string modelPath;
    float confidenceThreshold;
};

struct AppConfig {
    std::string windowName;
    std::string logLevel;
};

class ConfigManager {
public:
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    void loadConfig(const std::string& filePath);

    CameraConfig getCameraConfig() const;
    FaceDetectionConfig getFaceDetectionConfig() const;
    AppConfig getAppConfig() const;

private:
    ConfigManager();
    
    CameraConfig cameraConfig;
    FaceDetectionConfig faceDetectionConfig;
    AppConfig appConfig;
    
    std::mutex configMutex;
};
