#ifndef _FACE_DETECTOR_HPP_
#define _FACE_DETECTOR_HPP_

#include "detection/CascadeDetector.hpp"

#include "opencv2/objdetect.hpp"

class FaceDetectorCascade : public CascadeDetector
{
    public:
        FaceDetectorCascade(std::string& modelFilename);

        std::vector<cv::Rect> detect(cv::Mat& image) override;

        cv::Mat preProcessImage(cv::Mat& image);
    
    private:
        cv::CascadeClassifier detector;
};

#endif