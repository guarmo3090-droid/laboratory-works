#include "CameraProvider.hpp"
#include "Logger.hpp"
#include <iostream>

using namespace std;

CameraProvider::CameraProvider(int device) : deviceId(device) {}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
        Logger::getInstance().info("CameraProvider: Camera released.");
    }
}

bool CameraProvider::initialize() {
    Logger::getInstance().info("CameraProvider: Attempting to open camera ID " + to_string(deviceId));
    
    cap.open(deviceId);
    if (!cap.isOpened()) {
        Logger::getInstance().error("CameraProvider: Could not open camera with ID " + to_string(deviceId));
        return false;
    }
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    
    Logger::getInstance().info("CameraProvider: Camera initialized successfully (640x480).");
    return true;
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;
    } else {
        Logger::getInstance().error("CameraProvider: Attempted to get frame from closed camera.");
    }
    return frame;
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}
