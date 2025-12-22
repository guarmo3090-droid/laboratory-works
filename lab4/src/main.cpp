#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << "Starting Video Filters App..." << endl;
    cout << "Controls:" << endl;
    cout << " [1-8] Change Modes (Normal, Invert, Blur, Canny, Sobel, Threshold, Glitch, PiP)" << endl;
    cout << " [r/l] Rotate" << endl;
    cout << " [+/-] Zoom" << endl;
    cout << " [w/a/s/d] Move Crosshair" << endl;
    cout << " [Mouse Drag] Draw Rectangles" << endl;
    cout << " [ESC] Exit" << endl;

    CameraProvider camera(0);
    if (!camera.initialize()) {
        return -1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Video Filter App");
    
    display.setup(frameProcessor);

    while (camera.isOpened()) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) {
            cerr << "Captured empty frame" << endl;
            break;
        }

        frameProcessor.process(frame, keyProcessor);
        display.show(frame);

        int key = cv::waitKey(1);
        if (keyProcessor.processKey(key)) {
            break;
        }
    }

    return 0;
}
