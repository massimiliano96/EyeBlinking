#include "FaceDetected.hpp"

std::string FaceDetected::getName()
{
    return name;
}

cv::Rect FaceDetected::getRoi()
{
    return roi;
}

