#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "Logger.hpp"
#include <iostream>

using namespace std;

int main() {
    Logger& logger = Logger::getInstance();
    logger.info("Application Started");
    
    logger.setMinLevel(LogLevel::INFO);

    cout << "Starting Video Filters App..." << endl;
    cout << "Controls in logs..." << endl; 

    CameraProvider camera(0);
    if (!camera.initialize()) {
        logger.error("Main: Failed to initialize camera. Exiting.");
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Video Filter App");
    
    logger.info("Main: Components initialized. Starting loop.");
    display.setup(frameProcessor);

    while (camera.isOpened()) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            logger.warn("Main: Captured empty frame. Skipping.");
            continue;
        }

        frameProcessor.process(frame, keyProcessor);
        display.show(frame);

        int key = cv::waitKey(30);
        if (keyProcessor.processKey(key)) {
            logger.info("Main: ESC pressed. Stopping application.");
            break;
        }
    }
    
    logger.info("Application Stopped");
    return 0;
}
