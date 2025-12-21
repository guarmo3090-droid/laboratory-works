#include "CameraProvider.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp"

using namespace std;

CameraProvider::CameraProvider(int device) : deviceId(device) {}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) cap.release();
}

bool CameraProvider::initialize() {
    Logger::getInstance().info("CameraProvider: Opening camera ID " + to_string(deviceId));
    
    cap.open(deviceId);
    if (!cap.isOpened()) {
        Logger::getInstance().error("CameraProvider: Failed to open camera ID " + to_string(deviceId));
        return false;
    }
    
    CameraConfig cfg = ConfigManager::getInstance().getCameraConfig();
    
    cap.set(cv::CAP_PROP_FRAME_WIDTH, cfg.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, cfg.height);
    if (cfg.fps > 0) cap.set(cv::CAP_PROP_FPS, cfg.fps);
    
    Logger::getInstance().info("CameraProvider: Initialized with resolution " + 
                               to_string(cfg.width) + "x" + to_string(cfg.height));
    return true;
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    if (cap.isOpened()) cap >> frame;
    return frame;
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}
