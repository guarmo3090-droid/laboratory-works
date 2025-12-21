#include "ConfigManager.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>

using namespace std;

ConfigManager::ConfigManager() {
    cameraConfig = {0, 640, 480, 30};
    faceDetectionConfig = {false, "", "", 0.5f};
    appConfig = {"Video Filter App (Default)", "INFO"};
}

void ConfigManager::loadConfig(const string& filePath) {
    lock_guard<mutex> lock(configMutex);
    Logger& logger = Logger::getInstance();

    ifstream configFile(filePath);
    if (!configFile.is_open()) {
        logger.warn("ConfigManager: settings.json not found. Using default values.");
        return;
    }

    try {
        json j;
        configFile >> j;

        if (j.contains("camera")) {
            auto& c = j["camera"];
            cameraConfig.id = c.value("id", 0);
            cameraConfig.width = c.value("width", 640);
            cameraConfig.height = c.value("height", 480);
            cameraConfig.fps = c.value("fps", 30);
        }

        if (j.contains("face_detection")) {
            auto& f = j["face_detection"];
            faceDetectionConfig.enabled = f.value("enabled", false);
            faceDetectionConfig.protoPath = f.value("proto_path", "res/deploy.prototxt");
            faceDetectionConfig.modelPath = f.value("model_path", "res/res10_300x300_ssd_iter_140000.caffemodel");
            faceDetectionConfig.confidenceThreshold = f.value("confidence_threshold", 0.5f);
        }

        if (j.contains("app")) {
            appConfig.windowName = j["app"].value("window_name", "Video Filter App");
            
            string levelStr = j["app"].value("log_level", "INFO");
            appConfig.logLevel = levelStr;
            
            if (levelStr == "DEBUG") logger.setMinLevel(LogLevel::DEBUG);
            else if (levelStr == "WARN") logger.setMinLevel(LogLevel::WARN);
            else if (levelStr == "ERROR") logger.setMinLevel(LogLevel::ERROR);
            else logger.setMinLevel(LogLevel::INFO);
        }

        logger.info("ConfigManager: Configuration loaded successfully from " + filePath);

    } catch (const json::parse_error& e) {
        logger.error("ConfigManager: JSON parse error: " + string(e.what()));
        logger.warn("ConfigManager: Using default configuration due to parse error.");
    } catch (const exception& e) {
        logger.error("ConfigManager: Unknown error loading config: " + string(e.what()));
    }
}

CameraConfig ConfigManager::getCameraConfig() const { return cameraConfig; }
FaceDetectionConfig ConfigManager::getFaceDetectionConfig() const { return faceDetectionConfig; }
AppConfig ConfigManager::getAppConfig() const { return appConfig; }
