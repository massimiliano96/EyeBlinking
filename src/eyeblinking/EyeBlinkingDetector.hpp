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
    EyeBlinkingDetector(std::string modelsPath);

    void process(std::vector<cv::Mat>&);

    Dispatcher<BlinkDetected>& getBlinkEventDispacher();
    Dispatcher<FaceDetected>& getFaceEventDispacher();

protected:
    cv::Rect detectFace(cv::Mat&);
    std::vector<cv::Rect> detectEyes(cv::Mat&, cv::Rect&);
    bool checkEyeBlink(std::list<std::vector<cv::Rect>>&);

private:
    Dispatcher<BlinkDetected> blinkDetectedDispacher;
    Dispatcher<FaceDetected> faceDetectedDispacher;
    std::shared_ptr<CascadeDetector> faceDetector;
    std::shared_ptr<CascadeDetector> eyeDetector;
};

#endif