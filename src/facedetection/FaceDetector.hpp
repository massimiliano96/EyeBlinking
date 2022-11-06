#ifndef _FACE_DETECTOR_HPP_
#define _FACE_DETECTOR_HPP_

#include <opencv2/core.hpp>

struct Face 
{
    cv::Rect position;
}

struct Faces {
    std::vector<Face> faces;
}

class FaceDetector 
{    
    public:
        Faces detect(cv::Mat& image) = 0;
        
        bool isFacePositionStraight(cv::Mat image, cv::Rect facePosition) = 0;
}

#endif