#include "EyeBlinkingDetector.hpp"
#include "detection/cascade/impl/eyedetection/EyeDetectorCascade.hpp"
#include "detection/cascade/impl/facedetection/FaceDetectorCascade.hpp"
#include "events/EventType.hpp"
#include "events/FaceDetected.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/types_c.h>
#include <dlib/opencv.h>


const std::string faceDetectionModel = "haarcascade_frontalface_default.xml";
const std::string eyeDetectiongModel = "haarcascade_eye.xml";
const std::string landmarksDetectionModel = "shape_predictor_68_face_landmarks.dat";

void EyeBlinkingDetector::process(cv::Mat& frame)
{
    try
    {        
        cv::Rect faceRoi = this->detectFace(frame);
        this->detectEyes(frame, faceRoi);
        if(detectedEyes.size() >= 5)
        {
            bool isBlinkDetected = this->checkEyeBlink(frame, faceRoi);
            if (isBlinkDetected)
            {
                std::cout<<"Blink Detected"<<std::endl;
                BlinkDetected obj;
                BlinkDetectedEvent event(obj);
                this->blinkDetectedDispacher.post(event);
            }
            detectedEyes.pop_front();
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch( const std::runtime_error& error)
    {
        std::cerr<<error.what()<<std::endl;
    }
}

bool EyeBlinkingDetector::checkEyeBlink(cv::Mat& image, cv::Rect& faceRoi)
{
    IplImage iplImg = cvIplImage(image);
    dlib::cv_image<dlib::bgr_pixel> dlibColorImg(&iplImg);
    dlib::matrix<dlib::rgb_pixel> dlibMatrix;
    dlib::assign_image(dlibMatrix, dlibColorImg);
    const dlib::rectangle& faceRect = dlib::rectangle(faceRoi.x, faceRoi.y, faceRoi.x + faceRoi.width, faceRoi.y + faceRoi.height);
    dlib::full_object_detection shape = blinkingCascadeDetector(dlibMatrix, faceRect);
    return true;
}

cv::Rect EyeBlinkingDetector::detectFace(cv::Mat& image)
{
    cv::Rect roi(0, 0, image.cols, image.rows);
    cv::Mat preparedImage = faceCascadeDetector->preProcessImage(image, roi);
    std::vector<cv::Rect> detectedFaces = faceCascadeDetector->detect(preparedImage);
    switch (detectedFaces.size())
    {
        case 0:
        {
            throw std::runtime_error("No faces found");
            break;
        }
        case 1:
        {
            std::cout << "Face found" << std::endl;
            FaceDetected face(detectedFaces[0]);
            FaceDetectedEvent event(face);
            faceDetectedDispacher.post(event);
            break;
        }
        default:
        {
            throw std::runtime_error("Multiple faces found, show only one face!");
            break;
        }
    }
    return detectedFaces[0];
}

std::vector<cv::Rect> EyeBlinkingDetector::detectEyes(cv::Mat& image, cv::Rect& faceRoi)
{
    cv::Mat preparedImage = eyeCascadeDetector->preProcessImage(image, faceRoi);
    std::vector<cv::Rect> detectedEyes = eyeCascadeDetector->detect(preparedImage);
    switch (detectedEyes.size())
    {
        case 0:
        {
            std::cout << "No Eyes found" << std::endl;
            break;
        }
        default:
        {
            std::cout << "Eyes found" << std::endl;
            break;
        }
    }
    return detectedEyes;
}

EyeBlinkingDetector::EyeBlinkingDetector(std::string modelsPath)
{
    this->eyeCascadeDetector = std::make_shared<EyeDetectorCascade>(modelsPath + "/" + eyeDetectiongModel);
    this->faceCascadeDetector = std::make_shared<FaceDetectorCascade>(modelsPath + "/" + faceDetectionModel);
    dlib::deserialize(modelsPath + "/" + landmarksDetectionModel) >> blinkingCascadeDetector;
    
}

Dispatcher<BlinkDetected>& EyeBlinkingDetector::getBlinkEventDispacher() 
{
    return blinkDetectedDispacher;
}
Dispatcher<FaceDetected>& EyeBlinkingDetector::getFaceEventDispacher()  
{
    return faceDetectedDispacher;
}