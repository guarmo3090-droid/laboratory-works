#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();

    // Завантаження ваг та конфігурації
    void init(const string& configPath, const string& weightsPath);

    // Метод для передачі кадру в потік обробки
    void updateFrame(const cv::Mat& frame);

    // Метод для отримання результатів
    vector<cv::Rect> getDetectedFaces();

    // Керування потоком
    void start();
    void stop();

private:
    void workerLoop(); // Функція, що крутиться у фоні

    cv::dnn::Net net;
    thread worker;
    atomic<bool> running;
    mutex dataMutex; 

    cv::Mat currentFrame;             
    vector<cv::Rect> foundFaces;      
    bool hasNewFrame;
    bool modelLoaded;
};
