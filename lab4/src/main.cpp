#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp" 
#include <iostream>

using namespace std;

int main() {
    Logger& logger = Logger::getInstance();
    ConfigManager& configMgr = ConfigManager::getInstance();

    configMgr.loadConfig("settings.json");
    
    logger.info("Application Started");

    CameraConfig camConfig = configMgr.getCameraConfig();
    
    CameraProvider camera(camConfig.id); 
    
    if (!camera.initialize()) {
        logger.error("Main: Failed to initialize camera. Exiting.");
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    
    string winName = configMgr.getAppConfig().windowName;
    Display display(winName);
    
    display.setup(frameProcessor);

    while (camera.isOpened()) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            logger.warn("Main: Captured empty frame.");
            continue;
        }

        frameProcessor.process(frame, keyProcessor);
        display.show(frame);

        int key = cv::waitKey(30);
        if (keyProcessor.processKey(key)) {
            break;
        }
    }
    
    logger.info("Application Stopped");
    return 0;
}
