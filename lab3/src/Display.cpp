#include "Display.hpp"
#include <iostream>

using namespace std;

Display::Display(const string& name) : windowName(name) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

Display::~Display() {
    cv::destroyWindow(windowName);
}

void Display::setup(FrameProcessor& frameProcessor) {
    cv::setMouseCallback(windowName, FrameProcessor::onMouse, &frameProcessor);
    
    cv::createTrackbar("Brightness", windowName, 
                       frameProcessor.getBrightnessPtr(), 100, 
                       FrameProcessor::onTrackbar);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow(windowName, frame);
    }
}
