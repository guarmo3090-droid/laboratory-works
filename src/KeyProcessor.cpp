#include "KeyProcessor.hpp"
#include "Logger.hpp" 
#include <iostream>
#include <algorithm> 

using namespace std;

KeyProcessor::KeyProcessor() 
    : currentMode(ProcessingMode::None), shouldExit(false), 
      faceDetectionEnabled(false), 
      rotationAngle(0.0f), zoomLevel(1.0f), crossX(320), crossY(240) {
      Logger::getInstance().debug("KeyProcessor: Initialized.");
}

bool KeyProcessor::processKey(int key) {
    if (key == -1) return false;

    switch (key) {
        case 27: 
            return true;
        case '1': currentMode = ProcessingMode::None; Logger::getInstance().info("Mode: Normal"); break;
        case '2': currentMode = ProcessingMode::Invert; Logger::getInstance().info("Mode: Invert"); break;
        case '3': currentMode = ProcessingMode::Blur; Logger::getInstance().info("Mode: Blur"); break;
        case '4': currentMode = ProcessingMode::Canny; Logger::getInstance().info("Mode: Canny"); break;
        case '5': currentMode = ProcessingMode::Sobel; Logger::getInstance().info("Mode: Sobel"); break;
        case '6': currentMode = ProcessingMode::Threshold; Logger::getInstance().info("Mode: Threshold"); break;
        case '7': currentMode = ProcessingMode::Glitch; Logger::getInstance().info("Mode: Glitch"); break;
        case '8': currentMode = ProcessingMode::Pip; Logger::getInstance().info("Mode: PiP"); break;
        
        case 'f': 
        case 'F':
            faceDetectionEnabled = !faceDetectionEnabled;
            Logger::getInstance().info(string("Face Detection: ") + (faceDetectionEnabled ? "ON" : "OFF"));
            break;

        case 'r': rotationAngle += 5.0f; break;
        case 'l': rotationAngle -= 5.0f; break;
        case '+': case '=': zoomLevel += 0.1f; break;
        case '-': case '_': zoomLevel = max(0.1f, zoomLevel - 0.1f); break;
        
        case 'w': moveCross(0, -10); break;
        case 's': moveCross(0, 10); break;
        case 'a': moveCross(-10, 0); break;
        case 'd': moveCross(10, 0); break;
    }
    
    return false;
}

ProcessingMode KeyProcessor::getMode() const { return currentMode; }
float KeyProcessor::getRotation() const { return rotationAngle; }
float KeyProcessor::getZoom() const { return zoomLevel; }
cv::Point KeyProcessor::getCrossPos() const { return cv::Point(crossX, crossY); }

bool KeyProcessor::isFaceDetectionEnabled() const { 
    return faceDetectionEnabled; 
}

void KeyProcessor::moveCross(int dx, int dy) {
    crossX += dx;
    crossY += dy;
}
