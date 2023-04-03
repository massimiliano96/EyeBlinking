#ifndef _EYE_BLINKING_DETECTOR_H_
#define _EYE_BLINKING_DETECTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <dlib/image_processing.h>

#include "detection/cascade/CascadeDetector.hpp"

#include "events/BlinkDetected.hpp"
#include "events/Dispatcher.hpp"
#include "events/FaceDetected.hpp"

class EyeBlinkingDetector
{
public:
    EyeBlinkingDetector(std::string modelsPath);

    void process(cv::Mat&);

    Dispatcher<BlinkDetected>& getBlinkEventDispacher();
    Dispatcher<FaceDetected>& getFaceEventDispacher();

protected:
    cv::Rect detectFace(cv::Mat&);
    std::vector<cv::Rect> detectEyes(cv::Mat&, cv::Rect&);
    bool checkEyeBlink(cv::Mat&, cv::Rect&);

private:
    Dispatcher<BlinkDetected> blinkDetectedDispacher;
    Dispatcher<FaceDetected> faceDetectedDispacher;
    std::shared_ptr<CascadeDetector> faceCascadeDetector;
    std::shared_ptr<CascadeDetector> eyeCascadeDetector;
    dlib::shape_predictor blinkingCascadeDetector;

    std::list<std::vector<cv::Rect>> detectedEyes;
};

#endif