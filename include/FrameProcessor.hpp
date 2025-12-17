#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "KeyProcessor.hpp"
#include "FaceDetector.hpp"

using namespace std;

class FrameProcessor {
private:
    int brightness; 
    
    vector<cv::Rect> rectangles;
    cv::Point startPoint;
    bool isDragging;

    FaceDetector faceDetector;

public:
    FrameProcessor();
    
    void process(cv::Mat& frame, const KeyProcessor& keyProc);
    
    static void onMouse(int event, int x, int y, int flags, void* userdata);
    static void onTrackbar(int val, void* userdata);
    
    int* getBrightnessPtr();
    void handleMouse(int event, int x, int y, int flags);

private:
    void applyGlitch(cv::Mat& frame);
    void drawOverlay(cv::Mat& frame, const KeyProcessor& keyProc);
    void applyTransformations(cv::Mat& frame, const KeyProcessor& keyProc);
};
