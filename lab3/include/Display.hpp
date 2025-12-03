#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include "FrameProcessor.hpp"

class Display {
private:
    std::string windowName;

public:
    Display(const std::string& name);
    ~Display();
    
    // Ініціалізація вікна та прив'язка callback
    void setup(FrameProcessor& frameProcessor);
    
    // Відображення кадру
    void show(const cv::Mat& frame);
};
