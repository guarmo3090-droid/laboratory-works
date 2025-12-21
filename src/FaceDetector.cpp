#include "FaceDetector.hpp"
#include "Logger.hpp"       
#include <iostream>

using namespace std;


void FaceDetector::init(const string& configPath, const string& weightsPath) {
    try {
        net = cv::dnn::readNetFromCaffe(configPath, weightsPath);
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        modelLoaded = true;
        
        Logger::getInstance().info("FaceDetector: Model loaded successfully.");
        
    } catch (const cv::Exception& e) {
        Logger::getInstance().error("FaceDetector: Error loading model: " + string(e.what()));
    }
}

void FaceDetector::start() {
    if (!running) {
        running = true;
        worker = thread(&FaceDetector::workerLoop, this);
        
        Logger::getInstance().info("FaceDetector: Background thread started.");
    }
}

void FaceDetector::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
        
        Logger::getInstance().info("FaceDetector: Background thread stopped.");
    }
}
