#ifndef __FACE_DETECTED_HPP__
#define __FACE_DETECTED_HPP__

#include <opencv2/core/core.hpp>
#include <string>

class FaceDetected
{
public:
    explicit FaceDetected(cv::Rect r) : name("FaceDetected"), roi(r) {};

    ~FaceDetected() = default;

    std::string getName() const;

    cv::Rect getRoi() const;

private:
    std::string name = "";
    cv::Rect roi = {0, 0, 0, 0};
};

#endif /*  __BLINK_DETECTED_HPP__ */
