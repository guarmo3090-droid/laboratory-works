#include "CameraProvider.hpp"
#include <iostream>

using namespace std;

CameraProvider::CameraProvider(int device) : deviceId(device) {}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraProvider::initialize() {
    cap.open(deviceId);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera with ID " << deviceId << endl;
        return false;
    }
    // роздільна здатність 
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    return true;
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    if (cap.isOpened()) {
        cap >> frame;
    }
    return frame;
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}
