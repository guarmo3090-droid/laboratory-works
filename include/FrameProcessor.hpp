#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "KeyProcessor.hpp"

class FrameProcessor {
private:
    int brightness; 
    
    // Для малювання мишкою
    std::vector<cv::Rect> rectangles;
    cv::Point startPoint;
    bool isDragging;
    
    // Допоміжні зображення
    cv::Mat pipImage;

public:
    FrameProcessor();
    
    void process(cv::Mat& frame, const KeyProcessor& keyProc);
    
    // Статичні методи для callback
    static void onMouse(int event, int x, int y, int flags, void* userdata);
    static void onTrackbar(int val, void* userdata);
    
    // Геттер для адреси змінної яскравості (для створення трекбару)
    int* getBrightnessPtr();
    
    // Обробка подій миші (викликається зі статичного методу)
    void handleMouse(int event, int x, int y, int flags);

private:
    void applyGlitch(cv::Mat& frame);
    void drawOverlay(cv::Mat& frame, const KeyProcessor& keyProc);
    void applyTransformations(cv::Mat& frame, const KeyProcessor& keyProc);
};
