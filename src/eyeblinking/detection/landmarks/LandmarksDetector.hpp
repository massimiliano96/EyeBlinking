#ifndef _LANDMARKS_DETECTOR_HPP_
#define _LANDMARKS_DETECTOR_HPP_

#include <dlib/image_processing.h>
#include <opencv2/core.hpp>

class LandmarksDetector
{
public:
    LandmarksDetector() = default;

    virtual ~LandmarksDetector() = default;

    virtual bool detect(dlib::full_object_detection& shape) = 0;

    virtual dlib::full_object_detection preProcess(cv::Mat&, cv::Rect&) = 0;
};

#endif