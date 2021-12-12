#ifndef _EYE_BLINKING_DETECTOR_H_
#define _EYE_BLINKING_DETECTOR_H_

#include <dlib/opencv.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_io.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <dlib/image_processing.h>

#include "../events/Dispatcher.hpp"
#include "../events/EventType.hpp"
#include "../events/BlinkDetected.hpp"
#include "../events/FaceDetected.hpp"

/*
    This object is a detector aimed to detect if the subject filmed by the camera blinks or not, 
    for this goal it uses two models : 
    - A geometric approach using face landmarks
    - A Cascade pretrained model in OpenCV
    Moreover in order to find a face in the given image, it uses thew YOLO algorithm
*/
class EyeBlinkingDetector
{
    public:
        EyeBlinkingDetector(std::string models_path) {loadModels(models_path);} // The constructor takes in input the path of the model folder and call the method loadModels
    
        void detect(cv::Mat frame); // This method checks if the subject in front of the camera blinks or not

         // Event Dispatcher : These objects are respondible to register the observers of the corresponding event and also to post it
        Dispatcher<BlinkDetected> blink_dispacher;
        Dispatcher<FaceDetected> face_dispacher;
    
    private:
        bool loadModels(std::string& models_path);                 // this function reads the models from the input folder and store them into the correponding variables
        void CascadeBlink(cv::Mat mat, cv::Rect roi);              // this function execute the eye blinking detection using cascade classifier
        void RatioBlink(const dlib::full_object_detection& shape); // this function execute the eye blinking detection using face landmarks
        cv::Rect findFace(cv::Mat mat_bgr, float confidence_threshold, float nms_threshold, float threshold_area); // this method uses a YOLO networks in order to find a face
        inline dlib::rectangle openCVRectToDlib(cv::Rect r);

        dlib::shape_predictor face_shape_predictor_; //dlib model that extracts face landmarks

        cv::dnn::Net net_;            // YOLO for face detection
        cv::Size net_input_size_;     // input size of the network
        double net_input_scale_;      // input scale of the networks  

        
        cv::CascadeClassifier  detector_eyes; // Cascade Eye classifier

        bool result_cascade = false;          // result of the detection using the cascade approach
        bool result_ratio = false;            // result usign tha face landmarks approach

        std::list<bool> buffer_cascade = {};  // list of the partial result of the cascade methods (the classification is done using three con secutive frames)
        int buffer_size_eyes = 3;
        double buffer_classes_eyes[3] = {};   // vector of the partial results of the landamrks method (the classification is done using three con secutive frames)
        int buffer_count_eyes = 0;
};

#endif