#ifndef _EYE_DETECTOR_HPP_
#define _EYE_DETECTOR_HPP_

#include "detection/CascadeDetector.hpp"

#include <opencv2/objdetect.hpp>

class EyeDetectorCascade : public CascadeDetector
{
public:
    EyeDetectorCascade(const std::string& modelFilename);

    std::vector<cv::Rect> detect(cv::Mat& image, cv::Rect roi = cv::Rect(0, 0, 0, 0)) override;

    cv::Mat preProcessImage(cv::Mat& image, cv::Rect faceRoi);

private:
    cv::CascadeClassifier detector;
};

#endif