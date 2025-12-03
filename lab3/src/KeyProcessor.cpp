#include "KeyProcessor.hpp"
#include <iostream>
#include <algorithm> 

using namespace std;

KeyProcessor::KeyProcessor() 
    : currentMode(ProcessingMode::None), shouldExit(false), 
      rotationAngle(0.0f), zoomLevel(1.0f), crossX(320), crossY(240) {}

bool KeyProcessor::processKey(int key) {
    if (key == -1) return false; // Клавіша не натиснута

    // Навігація по режимах (клавіші 1-8)
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
        
        // Керування 
        case 'r': rotationAngle += 5.0f; break; // Rotate Right
        case 'l': rotationAngle -= 5.0f; break; // Rotate Left
        case '+': case '=': zoomLevel += 0.1f; break; // Zoom In
        case '-': case '_': zoomLevel = max(0.1f, zoomLevel - 0.1f); break; // Zoom Out
        
        // Рух хрестика (WASD)
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

void KeyProcessor::moveCross(int dx, int dy) {
    crossX += dx;
    crossY += dy;
}
