#ifndef _EYE_BLINKING_DETECTOR_HPP_
#define _EYE_BLINKING_DETECTOR_HPP_

#include "detection/landmarks/LandmarksDetector.hpp"
#include "utils/FixedSizeQueue.hpp"

const int ratioBufferSize = 5;

struct Eye
{
    dlib::point pointL;
    dlib::point pointLT;
    dlib::point pointRT;
    dlib::point pointR;
    dlib::point pointRB;
    dlib::point pointLB;
};

class EyeBlinkingDetector : public LandmarksDetector
{
public:
    explicit EyeBlinkingDetector(const std::string& modelFilename);

    bool detect(dlib::full_object_detection& shape) override;

    dlib::full_object_detection preProcess(cv::Mat&, cv::Rect&) override;

    double getDistance(dlib::point&, dlib::point&) const;

    bool checkEyeBlink(Eye& eye, FixedSizeQueue<double, ratioBufferSize>&) const;

private:
    dlib::shape_predictor detector;
    FixedSizeQueue<double, ratioBufferSize> ratiosBufferLeft;
    FixedSizeQueue<double, ratioBufferSize> ratiosBufferRight;
};

#endif