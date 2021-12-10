#ifndef _EYE_BLINKING_DETECTOR_H_
#define _EYE_BLINKING_DETECTOR_H_

#include <dlib/opencv.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_io.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
//#include <dlib/image_processing/full_object_detection.h>
#include <dlib/image_processing.h>

#include "../events/Dispatcher.hpp"
#include "../events/EventType.hpp"
#include "../events/BlinkDetected.hpp"
#include "../events/FaceDetected.hpp"

class EyeBlinkingDetector
{
    public:
        EyeBlinkingDetector(std::string models_path) {loadModels(models_path);}
    
        void detect(cv::Mat frame);

        Dispatcher<BlinkDetected> blink_dispacher;
        Dispatcher<FaceDetected> face_dispacher;
    
    private:
        bool loadModels(std::string& models_path);
        void CascadeBlink(cv::Mat mat, cv::Rect roi);
        void RatioBlink(const dlib::full_object_detection& shape);
        cv::Rect findFace(cv::Mat mat_bgr, float confidence_threshold, float nms_threshold, float threshold_area);
        dlib::rectangle openCVRectToDlib(cv::Rect r);

        dlib::shape_predictor face_shape_predictor_;

        cv::dnn::Net net_;
        cv::Size net_input_size_;
        double net_input_scale_;

        
        
        cv::CascadeClassifier detector_faces, detector_eyes;

        bool result_cascade = false;
        bool result_ratio = false;

        std::list<bool> buffer_cascade = {};
        int buffer_size_eyes = 3;
        double buffer_classes_eyes[3] = {};
        int buffer_count_eyes = 0;
        int count_eyes = 0;
        bool eye_blinking_result = false;
        double buffer_orizontal[3] = {};
};

#endif