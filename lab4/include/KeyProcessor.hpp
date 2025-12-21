#pragma once
#include <opencv2/opencv.hpp>

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
    bool faceDetectionEnabled; 
    
  
    float rotationAngle;
    float zoomLevel;
    int crossX, crossY;

public:
    KeyProcessor();
    
    bool processKey(int key);
    
    ProcessingMode getMode() const;
    float getRotation() const;
    float getZoom() const;
    cv::Point getCrossPos() const;
    
    bool isFaceDetectionEnabled() const; 
    
    void moveCross(int dx, int dy);
};
