#ifndef _EYE_DETECTOR_HPP_
#define _EYE_DETECTOR_HPP_

#include "detection/CascadeDetector.hpp"

#include "opencv2/objdetect.hpp"

class EyeDetectorCascade : public CascadeDetector
{
    public:
        EyeDetectorCascade(std::string& modelFilename);

        std::vector<cv::Rect> detect(cv::Mat& image) override;

        cv::Mat preProcessImage(cv::Mat& image, cv::Rect faceRoi);
    
    private:
        cv::CascadeClassifier detector;
};

#endif