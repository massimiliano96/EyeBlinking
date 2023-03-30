#include <iostream>
#include <filesystem>

#include <opencv2/opencv.hpp>
#include "EyeBlinkingDetector.hpp"
#include "events/BlinkDetected.hpp"
#include "events/FaceDetected.hpp"

#define MODELS_PATH "models"

// this function draws the face rectangle on the current frame when the FaceDetected event is posted
void onEventFaceDetected(const Event<FaceDetected>& event, cv::Mat& frame)
{
    try
    {
        FaceDetected obj = event.type();
        cv::Rect roi = obj.getRoi();
        cv::Scalar color = cv::Scalar(0, 255, 0);
        cv::rectangle(frame, roi, color, 5);
    }
    catch (cv::Exception& e)
    {
        std::cout << "Exception Thrown : " << e.what() << std::endl;
    }
}
// this function write "BLINK DETECTED" on the current frame when the BlinkDetected event is posted
void onEventBlinkDetected(const Event<BlinkDetected>& event, cv::Mat& frame)
{
    try
    {
        BlinkDetected obj = event.type();
        cv::Scalar color = cv::Scalar(0, 255, 0);
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

    EyeBlinkingDetector detector(workingDirectory + "/" + MODELS_PATH);

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    std::vector<cv::Mat> imagesBuffer;

    while (1)
    {
        try
        {
            cv::Mat frame;
            cap >> frame;

            detector.getFaceEventDispacher().subscribe(std::bind(&onEventFaceDetected, std::placeholders::_1, frame));
            detector.getBlinkEventDispacher().subscribe(std::bind(&onEventBlinkDetected, std::placeholders::_1, frame));

            // If the frame is empty, break immediately
            if (frame.empty())
                break;

            imagesBuffer.push_back(frame);
            cv::imshow("Frame", frame);

            if (imagesBuffer.size() >= 5)
            {
                detector.process(imagesBuffer);
                imagesBuffer.resize(0);
            }

            // Press  ESC on keyboard to exit
            char c = (char)cv::waitKey(25);
            if (c == 27)
                break;
        }
        catch (cv::Exception e)
        {
            std::cout << "Exception Thrown : " << e.what() << std::endl;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
