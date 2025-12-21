#include "FaceDetector.hpp"
#include <iostream>
#include <chrono> 

FaceDetector::FaceDetector() : running(false), hasNewFrame(false), modelLoaded(false) {}

FaceDetector::~FaceDetector() {
    stop();
}

void FaceDetector::init(const string& configPath, const string& weightsPath) {
    try {
        net = cv::dnn::readNetFromCaffe(configPath, weightsPath);
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        
        modelLoaded = true;
        cout << "[FaceDetector] Model loaded successfully." << endl;
    } catch (const cv::Exception& e) {
        cerr << "[FaceDetector] Error loading model: " << e.what() << endl;
    }
}

void FaceDetector::start() {
    if (!running && modelLoaded) {
        running = true;
        worker = thread(&FaceDetector::workerLoop, this);
        cout << "[FaceDetector] Background thread started." << endl;
    }
}

void FaceDetector::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
        cout << "[FaceDetector] Background thread stopped." << endl;
    }
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    if (!running) return;
    
    lock_guard<mutex> lock(dataMutex);
    if (frame.empty()) return;
    frame.copyTo(currentFrame);
    hasNewFrame = true;
}

vector<cv::Rect> FaceDetector::getDetectedFaces() {
    lock_guard<mutex> lock(dataMutex);
    return foundFaces;
}

void FaceDetector::workerLoop() {
    cv::Mat processFrame;

    while (running) {
        bool workToDo = false;

        // 1. take the frame
        {
            lock_guard<mutex> lock(dataMutex);
            if (hasNewFrame && !currentFrame.empty()) {
                currentFrame.copyTo(processFrame);
                hasNewFrame = false;
                workToDo = true;
            }
        }

        if (workToDo) {
       
            cv::Mat blob = cv::dnn::blobFromImage(processFrame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detection = net.forward();

            // delay
            this_thread::sleep_for(chrono::milliseconds(500)); 
          

            vector<cv::Rect> tempFaces;
            cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

            for (int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);

                if (confidence > 0.5f) {
                    int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * processFrame.cols);
                    int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * processFrame.rows);
                    int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * processFrame.cols);
                    int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * processFrame.rows);

                    tempFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
                }
            }

            // 3. save result
            {
                lock_guard<mutex> lock(dataMutex);
                foundFaces = tempFaces;
            }
        } else {
            // economize CPU
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}
