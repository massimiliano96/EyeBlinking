#include <filesystem>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "DetectionOrchestrator.hpp"
#include "events/BlinkDetected.hpp"
#include "events/EyeDetected.hpp"
#include "events/FaceDetected.hpp"

#define MODELS_PATH "models"

// this function draws the face rectangle on the current frame when the FaceDetected event is posted
void onEventFaceDetected(const Event<FaceDetected>& event, cv::Mat& frame)
{
    try
    {
        FaceDetected obj = event.type();
        cv::Rect roi = obj.getRoi();
        cv::Scalar color(0, 255, 0);
        cv::rectangle(frame, roi, color, 5);
    }
    catch (cv::Exception& e)
    {
        std::cout << "Exception Thrown : " << e.what() << std::endl;
    }
}

void onEventEyeDetected(const Event<EyeDetected>& event, cv::Mat& frame)
{
    try
    {
        EyeDetected obj = event.type();
        cv::Scalar color(0, 255, 0);
        std::vector<cv::Rect> detectedEyes = obj.getEyes();
        for (const cv::Rect& eye : detectedEyes)
            cv::rectangle(frame, eye, color, 3);
    }
    catch (cv::Exception& e)
    {
        std::cout << "Exception Thrown : " << e.what() << std::endl;
    }
}

void onEventBlinkDetected(const Event<BlinkDetected>& event, cv::Mat& frame)
{
    try
    {
        BlinkDetected obj = event.type();
        cv::Scalar color(0, 255, 0);
        cv::putText(frame, "BLINK DETECTED!!", cv::Point(frame.rows / 2, 50), cv::FONT_HERSHEY_DUPLEX, 1, color, 2, false);
    }
    catch (cv::Exception& e)
    {
        std::cout << "Exception Thrown : " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::string workingDirectory = std::filesystem::current_path();

    DetectionOrchestrator detector(workingDirectory + "/" + MODELS_PATH);

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while (true)
    {
        try
        {
            cv::Mat frame;
            cap >> frame;

            detector.getFaceEventDispacher().subscribe(std::bind(&onEventFaceDetected, std::placeholders::_1, frame));
            detector.getBlinkEventDispacher().subscribe(std::bind(&onEventBlinkDetected, std::placeholders::_1, frame));
            detector.getEyeEventDispacher().subscribe(std::bind(&onEventEyeDetected, std::placeholders::_1, frame));

            // If the frame is empty, break immediately
            if (frame.empty())
                break;

            detector.process(frame);

            detector.getFaceEventDispacher().disconnect();
            detector.getBlinkEventDispacher().disconnect();

            cv::imshow("Frame", frame);

            // Press  ESC on keyboard to exit
            char c = (char)cv::waitKey(25);
            if (c == 27)
                break;
        }
        catch (cv::Exception& e)
        {
            std::cout << "Exception Thrown : " << e.what() << std::endl;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
