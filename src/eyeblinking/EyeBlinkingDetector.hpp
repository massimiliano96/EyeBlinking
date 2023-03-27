#ifndef _EYE_BLINKING_DETECTOR_H_
#define _EYE_BLINKING_DETECTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "events/BlinkDetected.hpp"
#include "events/Dispatcher.hpp"
#include "events/EventType.hpp"
#include "events/FaceDetected.hpp"

class EyeBlinkingDetector
{
public:
    EyeBlinkingDetector(std::string models_path)
    {
        loadModels(models_path);
    } // The constructor takes in input the path of the model folder and call the method loadModels

    void detect(cv::Mat frame); // This method checks if the subject in front of the camera blinks or not

    // Event Dispatcher : These objects are respondible to register the observers of the corresponding event and also to post it
    Dispatcher<BlinkDetected> blink_dispacher;
    Dispatcher<FaceDetected> face_dispacher;

private:
    bool loadModels(std::string& models_path);    // this function reads the models from the input folder and store them into the correponding variables
    void CascadeBlink(cv::Mat mat, cv::Rect roi); // this function execute the eye blinking detection using cascade classifier
    cv::Rect findFace(cv::Mat mat_bgr, float confidence_threshold, float nms_threshold, float threshold_area);

    cv::dnn::Net net_;        // YOLO for face detection
    cv::Size net_input_size_; // input size of the network
    double net_input_scale_;  // input scale of the networks

    cv::CascadeClassifier detector_eyes; // Cascade Eye classifier

    bool result_cascade = false; // result of the detection using the cascade approach
    bool result_ratio = false;   // result usign tha face landmarks approach

    std::list<bool> buffer_cascade = {}; // list of the partial result of the cascade methods (the classification is done using three con secutive frames)
    int buffer_size_eyes = 3;
};

#endif