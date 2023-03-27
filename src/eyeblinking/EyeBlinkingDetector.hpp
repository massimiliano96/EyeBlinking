#ifndef _EYE_BLINKING_DETECTOR_H_
#define _EYE_BLINKING_DETECTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "detection/CascadeDetector.hpp"

#include "events/BlinkDetected.hpp"
#include "events/Dispatcher.hpp"
#include "events/FaceDetected.hpp"

class EyeBlinkingDetector
{
public:
    EyeBlinkingDetector(std::string models_path);

    void detect(cv::Mat frames);

private:
    Dispatcher<BlinkDetected> blinkDetectedDispacher;
    Dispatcher<FaceDetected> faceDetectedDispacher;
    std::vector<std::shared_ptr<CascadeDetector>> detectors;
};

#endif