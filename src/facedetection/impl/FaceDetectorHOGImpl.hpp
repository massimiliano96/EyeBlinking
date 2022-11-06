#ifndef _FACE_DETECTOR_HOG_IMPL_HPP_
#define _FACE_DETECTOR_HOG_IMPL_HPP_

#include "FaceDetected.hpp"

class FaceDetectorHogImpl : public FaceDetector
{
    public:
        FaceDetectorHogImpl(std::string& modelFilename);

        Faces detect(cv::Mat& image) override;
        
        bool isFacePositionStraight(cv::Mat image, cv::Rect facePosition) override;
    
    private:

        cv::CascadeClassifier detector;

}

#endif