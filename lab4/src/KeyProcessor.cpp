#include "KeyProcessor.hpp"
#include <algorithm> 

KeyProcessor::KeyProcessor() 
    : currentMode(ProcessingMode::None), shouldExit(false), faceDetectionMode(false),
      rotationAngle(0.0f), zoomLevel(1.0f), crossX(320), crossY(240) {}

bool KeyProcessor::processKey(int key) {
    if (key == -1) return false; 

    if (key == 'f' || key == 'F') {
        faceDetectionMode = !faceDetectionMode;
        cout << "Face Detection: " << (faceDetectionMode ? "ON" : "OFF") << endl;
        return false;
    }

    switch (key) {
        case 27: // ESC
            return true;
        case '1': currentMode = ProcessingMode::None; break;
        case '2': currentMode = ProcessingMode::Invert; break;
        case '3': currentMode = ProcessingMode::Blur; break;
        case '4': currentMode = ProcessingMode::Canny; break;
        case '5': currentMode = ProcessingMode::Sobel; break;
        case '6': currentMode = ProcessingMode::Threshold; break;
        case '7': currentMode = ProcessingMode::Glitch; break;
        case '8': currentMode = ProcessingMode::Pip; break;
        
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
bool KeyProcessor::isFaceDetectionEnabled() const { return faceDetectionMode; }
float KeyProcessor::getRotation() const { return rotationAngle; }
float KeyProcessor::getZoom() const { return zoomLevel; }
cv::Point KeyProcessor::getCrossPos() const { return cv::Point(crossX, crossY); }

void KeyProcessor::moveCross(int dx, int dy) {
    crossX += dx;
    crossY += dy;
}
