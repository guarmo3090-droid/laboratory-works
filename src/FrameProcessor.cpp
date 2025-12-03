#include "FrameProcessor.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Додано для std::clamp

using namespace std;

FrameProcessor::FrameProcessor() 
    : brightness(50), isDragging(false) {
    // pipImage більше не потрібен, тому видаляємо його ініціалізацію
}

int* FrameProcessor::getBrightnessPtr() {
    return &brightness;
}

void FrameProcessor::onMouse(int event, int x, int y, int flags, void* userdata) {
    FrameProcessor* processor = static_cast<FrameProcessor*>(userdata);
    if (processor) {
        processor->handleMouse(event, x, y, flags);
    }
}

void FrameProcessor::onTrackbar(int, void*) {
    // Значення автоматично оновлюється через вказівник
}

void FrameProcessor::handleMouse(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        startPoint = cv::Point(x, y);
        isDragging = true;
    } else if (event == cv::EVENT_MOUSEMOVE && isDragging) {
    } else if (event == cv::EVENT_LBUTTONUP) {
        isDragging = false;
        cv::Point endPoint(x, y);
        rectangles.push_back(cv::Rect(startPoint, endPoint));
    }
}

void FrameProcessor::process(cv::Mat& frame, const KeyProcessor& keyProc) {
    if (frame.empty()) return;

    // Застосування яскравості
    double beta = (brightness - 50.0) * 2.0;
    frame.convertTo(frame, -1, 1.0, beta);

    // Геометричні трансформації
    applyTransformations(frame, keyProc);

    // Фільтри
    cv::Mat processed;
    switch (keyProc.getMode()) {
        case ProcessingMode::Invert:
            cv::bitwise_not(frame, frame);
            break;
        case ProcessingMode::Blur:
            cv::GaussianBlur(frame, frame, cv::Size(15, 15), 0);
            break;
        case ProcessingMode::Canny:
            cv::cvtColor(frame, processed, cv::COLOR_BGR2GRAY);
            cv::Canny(processed, processed, 50, 150);
            cv::cvtColor(processed, frame, cv::COLOR_GRAY2BGR);
            break;
        case ProcessingMode::Sobel: {
            cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
            cv::cvtColor(frame, processed, cv::COLOR_BGR2GRAY);
            cv::Sobel(processed, grad_x, CV_16S, 1, 0, 3);
            cv::Sobel(processed, grad_y, CV_16S, 0, 1, 3);
            cv::convertScaleAbs(grad_x, abs_grad_x);
            cv::convertScaleAbs(grad_y, abs_grad_y);
            cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, processed);
            cv::cvtColor(processed, frame, cv::COLOR_GRAY2BGR);
            break;
        }
        case ProcessingMode::Threshold:
            cv::cvtColor(frame, processed, cv::COLOR_BGR2GRAY);
            cv::threshold(processed, processed, 128, 255, cv::THRESH_BINARY);
            cv::cvtColor(processed, frame, cv::COLOR_GRAY2BGR);
            break;
        case ProcessingMode::Glitch:
            applyGlitch(frame);
            break;
            
        case ProcessingMode::Pip: {
            // === ОНОВЛЕНА ЛОГІКА PiP (Scope Zoom) ===
            cv::Point cross = keyProc.getCrossPos();
            
            int scopeSize = 100; // Розмір області, яку вирізаємо (в пікселях)
            int pipScale = 3;    // Коефіцієнт збільшення (зум)
            int pipSize = scopeSize * pipScale; // Розмір вікна на екрані

            // 1. Визначаємо координати області вирізання (ROI)
            // std::clamp гарантує, що ми не вийдемо за межі зображення
            int x1 = std::clamp(cross.x - scopeSize / 2, 0, frame.cols - scopeSize);
            int y1 = std::clamp(cross.y - scopeSize / 2, 0, frame.rows - scopeSize);
            
            // 2. Вирізаємо та збільшуємо
            cv::Rect srcRect(x1, y1, scopeSize, scopeSize);
            cv::Mat crop = frame(srcRect);
            
            cv::Mat zoomedPip;
            // Використовуємо INTER_NEAREST для ефекту "піксельного" прицілу або INTER_LINEAR для гладкості
            cv::resize(crop, zoomedPip, cv::Size(pipSize, pipSize), 0, 0, cv::INTER_NEAREST);
            
            // 3. Декор: рамка та перехрестя всередині зуму
            cv::rectangle(zoomedPip, cv::Rect(0, 0, pipSize, pipSize), cv::Scalar(0, 255, 0), 3);
            // Вертикальна лінія
            cv::line(zoomedPip, cv::Point(pipSize/2, 0), cv::Point(pipSize/2, pipSize), cv::Scalar(0, 255, 0), 1);
            // Горизонтальна лінія
            cv::line(zoomedPip, cv::Point(0, pipSize/2), cv::Point(pipSize, pipSize/2), cv::Scalar(0, 255, 0), 1);

            // 4. Розміщення на екрані (Правий верхній кут за замовчуванням)
            int destX = frame.cols - pipSize - 20;
            int destY = 20;
            
            // Якщо основний приціл заходить під вікно PiP, переміщаємо вікно в лівий кут
            if (cross.x > destX - 50 && cross.y < destY + pipSize + 50) {
                destX = 20;
            }
            
            // Накладаємо, перевіряючи межі
            if (destX >= 0 && destY >= 0 && destX + pipSize <= frame.cols && destY + pipSize <= frame.rows) {
                 zoomedPip.copyTo(frame(cv::Rect(destX, destY, pipSize, pipSize)));
            }
            break;
        }
            
        default:
            break;
    }

    // Малювання прямокутників
    for (const auto& rect : rectangles) {
        cv::rectangle(frame, rect, cv::Scalar(0, 255, 255), 2);
    }

    // Оверлей
    drawOverlay(frame, keyProc);
}

void FrameProcessor::applyTransformations(cv::Mat& frame, const KeyProcessor& keyProc) {
    float angle = keyProc.getRotation();
    float scale = keyProc.getZoom();

    if (abs(angle) > 0.1 || abs(scale - 1.0f) > 0.01) {
        cv::Point2f center(frame.cols / 2.0f, frame.rows / 2.0f);
        cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, scale);
        cv::warpAffine(frame, frame, rotMat, frame.size());
    }
}

void FrameProcessor::applyGlitch(cv::Mat& frame) {
    vector<cv::Mat> channels;
    cv::split(frame, channels);
    
    int shift = 10;
    cv::Mat shiftedR = cv::Mat::zeros(channels[2].size(), channels[2].type());
    cv::Rect roiSrc(0, 0, channels[2].cols - shift, channels[2].rows);
    cv::Rect roiDst(shift, 0, channels[2].cols - shift, channels[2].rows);
    channels[2](roiSrc).copyTo(shiftedR(roiDst));
    
    channels[2] = shiftedR;
    cv::merge(channels, frame);
}

void FrameProcessor::drawOverlay(cv::Mat& frame, const KeyProcessor& keyProc) {
    // Хрестик
    cv::Point cross = keyProc.getCrossPos();
    cv::drawMarker(frame, cross, cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 20, 2);

    // Текст статистики
    string info = "Mode: " + to_string((int)keyProc.getMode()) + 
                  " | Zoom: " + to_string(keyProc.getZoom()).substr(0,3) + 
                  " | Rot: " + to_string((int)keyProc.getRotation());
    
    static double lastTime = 0;
    double currentTime = cv::getTickCount();
    double fps = cv::getTickFrequency() / (currentTime - lastTime);
    lastTime = currentTime;
    
    cv::putText(frame, "FPS: " + to_string((int)fps), cv::Point(10, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, info, cv::Point(10, frame.rows - 20), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    
    cv::Scalar avgPixel = cv::mean(frame);
    string intensity = "Avg RGB: " + to_string((int)avgPixel[2]) + " " +
                       to_string((int)avgPixel[1]) + " " + to_string((int)avgPixel[0]);
    cv::putText(frame, intensity, cv::Point(10, 60), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
}
