#ifndef _DETECTION_ORCHESTRATOR_HPP_
#define _DETECTION_ORCHESTRATOR_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <dlib/image_processing.h>

#include "detection/cascade/CascadeDetector.hpp"
#include "detection/landmarks/LandmarksDetector.hpp"

#include "events/BlinkDetected.hpp"
#include "events/Dispatcher.hpp"
#include "events/EyeDetected.hpp"
#include "events/FaceDetected.hpp"

class DetectionOrchestrator
{
public:
    explicit DetectionOrchestrator(const std::string& modelsPath);

    void process(cv::Mat&);

    Dispatcher<BlinkDetected>& getBlinkEventDispacher();
    Dispatcher<FaceDetected>& getFaceEventDispacher();
    Dispatcher<EyeDetected>& getEyeEventDispacher();

protected:
    cv::Rect detectFace(cv::Mat&);
    std::vector<cv::Rect> detectEyes(cv::Mat&, cv::Rect&);
    bool checkEyeBlink(cv::Mat&, cv::Rect&);

private:
    Dispatcher<BlinkDetected> blinkDetectedDispacher;
    Dispatcher<FaceDetected> faceDetectedDispacher;
    Dispatcher<EyeDetected> eyeDetectedDispacher;

    std::shared_ptr<CascadeDetector> faceCascadeDetector;
    std::shared_ptr<CascadeDetector> eyeCascadeDetector;
    std::shared_ptr<LandmarksDetector> blinkingLandmarksDetector;
};

#endif