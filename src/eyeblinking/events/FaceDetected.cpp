#include "FaceDetected.hpp"

std::string FaceDetected::getName() const
{
    return name;
}

cv::Rect FaceDetected::getRoi() const
{
    return roi;
}
