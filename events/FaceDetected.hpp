#ifndef __FACE_DETECTED_HPP__
#define __FACE_DETECTED_HPP__

#include <string>
#include <opencv2/core/core.hpp>

class FaceDetected 
{
    public:

    FaceDetected (cv::Rect r)
    {
        name = "FaceDetected";
        roi = r;
    };

    ~FaceDetected() = default;

    std::string getName();

    cv::Rect getRoi();

    private:
    
    std::string name = "";
    cv::Rect roi = {0,0,0,0};
};

#endif /*  __BLINK_DETECTED_HPP__ */
