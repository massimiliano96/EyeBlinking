#include <opencv2/opencv.hpp>
#include <iostream>
#include "../tools/EyeBlinkingDetector.hpp"
#include "../events/FaceDetected.hpp"
#include "../events/BlinkDetected.hpp"

#define MODELS_PATH "/usr/share/phygitalai/models"


void onEventFaceDetected(const Event<FaceDetected>& event, cv::Mat& frame)
{
    FaceDetected obj = event.type();
    cv::Rect roi = obj.getRoi();
    cv::Scalar color = cv::Scalar(0, 255, 0);
    cv::rectangle(frame, roi, color, 5);
}

void onEventBlinkDetected(const Event<BlinkDetected>& event, cv::Mat& frame)
{
    BlinkDetected obj = event.type();
    cv::Scalar color = cv::Scalar(0, 255, 0);
    cv::putText(frame, "BLINK DETECTED!!", cv::Point(frame.rows/2, 50),cv::FONT_HERSHEY_DUPLEX,1,color,2,false);
}


int main(int argc, char *argv[])
{
    try
    {
        EyeBlinkingDetector detector(MODELS_PATH);

        cv::VideoCapture cap(0);
        if(!cap.isOpened()){
            std::cout << "Error opening video stream or file" << std::endl;
            return -1;
        }

        while(1)
        {
            cv::Mat frame;
            cap >> frame;

            detector.face_dispacher.subscribe(std::bind(&onEventFaceDetected,std::placeholders::_1, frame));
            detector.blink_dispacher.subscribe(std::bind(&onEventBlinkDetected,std::placeholders::_1, frame));
            
            //If the frame is empty, break immediately
            if (frame.empty())
                break;
            
            detector.detect(frame);
            cv::imshow( "Frame", frame);
            // Press  ESC on keyboard to exit
            char c=(char)cv::waitKey(25);
            if(c==27)
                break;
        }
        cap.release();
        cv::destroyAllWindows();
    }
    catch(cv::Exception e)
    {
        std::cerr<<"Exception Thrown : "<<e.what()<<std::endl;
    }

    return 0;
}
