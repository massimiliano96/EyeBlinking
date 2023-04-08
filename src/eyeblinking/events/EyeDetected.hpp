#ifndef __EYE_DETECTED_HPP__
#define __EYE_DETECTED_HPP__

#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

class EyeDetected
{
public:
    explicit EyeDetected(const std::vector<cv::Rect>& detectedEyes) : name("EyeDetected"), eyes(detectedEyes) {};

    ~EyeDetected() = default;

    std::string getName() const;

    std::vector<cv::Rect> getEyes() const;

private:
    std::string name = "";
    std::vector<cv::Rect> eyes;
};

#endif /*  __EYE_DETECTED_HPP__ */
