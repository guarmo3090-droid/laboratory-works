#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class CameraProvider {
private:
    cv::VideoCapture cap;
    int deviceId;

public:
    CameraProvider(int device = 0);
    ~CameraProvider();
    
    bool initialize();
    cv::Mat getFrame();
    bool isOpened() const;
};
