#include <filesystem>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "DetectionOrchestrator.hpp"
#include "events/BlinkDetected.hpp"
#include "events/EyeDetected.hpp"
#include "events/FaceDetected.hpp"

#define MODELS_PATH "models"

// function to draw the face rectangle on the current frame
void drawFaceRect(const cv::Rect& roi, cv::Mat& frame)
{
    cv::Scalar color(0, 255, 0);
    cv::rectangle(frame, roi, color, 5);
}

// function to draw the eye rectangles on the current frame
void drawEyeRects(const std::vector<cv::Rect>& detectedEyes, cv::Mat& frame)
{
    cv::Scalar color(0, 255, 0);
    for (const cv::Rect& eye : detectedEyes)
    {
        cv::rectangle(frame, eye, color, 3);
    }
}

// function to draw the blink detection message on the current frame
void drawBlinkDetectionMessage(cv::Mat& frame)
{
    cv::Scalar color(0, 255, 0);
    cv::putText(frame, "BLINK DETECTED!!", cv::Point(frame.rows / 2, 50), cv::FONT_HERSHEY_DUPLEX, 1, color, 2, false);
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

    cv::Mat frame;

    // subscribe to events once at the beginning of the execution
    detector.getFaceEventDispacher().subscribe(
        [&frame](const Event<FaceDetected>& event)
        {
            try
            {
                drawFaceRect(event.type().getRoi(), frame);
            }
            catch (cv::Exception& e)
            {
                std::cout << "Exception Thrown : " << e.what() << std::endl;
            }
        });

    detector.getEyeEventDispacher().subscribe(
        [&frame](const Event<EyeDetected>& event)
        {
            try
            {
                drawEyeRects(event.type().getEyes(), frame);
            }
            catch (cv::Exception& e)
            {
                std::cout << "Exception Thrown : " << e.what() << std::endl;
            }
        });

    detector.getBlinkEventDispacher().subscribe(
        [&frame](const Event<BlinkDetected>& event)
        {
            try
            {
                drawBlinkDetectionMessage(frame);
            }
            catch (cv::Exception& e)
            {
                std::cout << "Exception Thrown : " << e.what() << std::endl;
            }
        });

    while (true)
    {
        try
        {
            cap >> frame;

            // If the frame is empty, break immediately
            if (frame.empty())
                break;

            detector.process(frame);

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
    // disconnect events at
    detector.getFaceEventDispacher().disconnect();
    detector.getBlinkEventDispacher().disconnect();
    detector.getEyeEventDispacher().disconnect();
}
