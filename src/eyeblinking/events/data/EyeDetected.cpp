#include "EyeDetected.hpp"

std::string EyeDetected::getName() const
{
    return name;
}

std::vector<cv::Rect> EyeDetected::getEyes() const
{
    return eyes;
}