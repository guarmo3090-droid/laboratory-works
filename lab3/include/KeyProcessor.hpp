#pragma once
#include <opencv2/opencv.hpp>

// Режими обробки
enum class ProcessingMode {
    None,
    Invert,
    Blur,
    Canny,
    Sobel,
    Threshold,
    Glitch,
    Pip // Picture in Picture
};

class KeyProcessor {
private:
    ProcessingMode currentMode;
    bool shouldExit;
    
    // Стан для інтерактивних змін 
    float rotationAngle;
    float zoomLevel;
    int crossX, crossY; // Координати хрестика

public:
    KeyProcessor();
    
    // Повертає true, якщо натиснуто ESC (вихід)
    bool processKey(int key);
    
    ProcessingMode getMode() const;
    float getRotation() const;
    float getZoom() const;
    cv::Point getCrossPos() const;
    void moveCross(int dx, int dy);
};
