#ifndef _FACE_DETECTOR_HPP_
#define _FACE_DETECTOR_HPP_

#include "detection/cascade/CascadeDetector.hpp"

#include "opencv2/objdetect.hpp"

class FaceDetectorCascade : public CascadeDetector
{
public:
    explicit FaceDetectorCascade(const std::string& modelFilename);

    std::vector<cv::Rect> detect(cv::Mat& image) override;

    cv::Mat preProcess(cv::Mat& image, cv::Rect& roi) override;

    std::vector<cv::Rect> postProcess(std::vector<cv::Rect>&, cv::Rect&) override;

private:
    cv::CascadeClassifier detector;
};

#endif