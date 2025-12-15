#pragma once
#include <opencv2/opencv.hpp>
#include <iostream> 

using namespace std;

// Режими обробки
enum class ProcessingMode {
    None,
    Invert,
    Blur,
    Canny,
    Sobel,
    Threshold,
    Glitch,
    Pip 
};

class KeyProcessor {
private:
    ProcessingMode currentMode;
    bool shouldExit;
    bool faceDetectionMode; 
    
    float rotationAngle;
    float zoomLevel;
    int crossX, crossY; 

public:
    KeyProcessor();
    bool processKey(int key);
    ProcessingMode getMode() const;
    bool isFaceDetectionEnabled() const;

    float getRotation() const;
    float getZoom() const;
    cv::Point getCrossPos() const;
    void moveCross(int dx, int dy);
};
