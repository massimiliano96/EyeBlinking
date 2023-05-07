#include "DetectionOrchestrator.hpp"
#include "detection/cascade/impl/eyedetection/EyeDetectorCascade.hpp"
#include "detection/cascade/impl/facedetection/FaceDetectorCascade.hpp"
#include "detection/landmarks/impl/eyeblinking/EyeBlinkingDetector.hpp"
#include "events/EventTypes.hpp"
#include "events/data/FaceDetected.hpp"

#include <dlib/opencv.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs.hpp>

const std::string faceDetectionModel = "haarcascade_frontalface_default.xml";
const std::string eyeDetectiongModel = "haarcascade_eye.xml";
const std::string landmarksDetectionModel = "shape_predictor_68_face_landmarks.dat";

void DetectionOrchestrator::process(cv::Mat& frame)
{
    try
    {
        cv::Rect faceRoi = this->detectFace(frame);
        this->detectEyes(frame, faceRoi);
        bool isBlinkDetected = this->checkEyeBlink(frame, faceRoi);

        if (isBlinkDetected)
        {
            BlinkDetected obj;
            BlinkDetectedEvent event(obj);
            this->blinkDetectedDispacher.post(event);
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << error.what() << std::endl;
    }
}

bool DetectionOrchestrator::checkEyeBlink(cv::Mat& image, cv::Rect& faceRoi)
{
    dlib::full_object_detection faceLandmarks = blinkingLandmarksDetector->preProcess(image, faceRoi);
    return blinkingLandmarksDetector->detect(faceLandmarks);
}

cv::Rect DetectionOrchestrator::detectFace(cv::Mat& image)
{
    cv::Rect roi(0, 0, image.cols, image.rows);
    cv::Mat preparedImage = faceCascadeDetector->preProcess(image, roi);
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

std::vector<cv::Rect> DetectionOrchestrator::detectEyes(cv::Mat& image, cv::Rect& faceRoi)
{
    cv::Mat preparedImage = eyeCascadeDetector->preProcess(image, faceRoi);
    std::vector<cv::Rect> detectedEyes = eyeCascadeDetector->detect(preparedImage);
    detectedEyes = eyeCascadeDetector->postProcess(detectedEyes, faceRoi);
    switch (detectedEyes.size())
    {
        case 0:
        {
            std::cout << "No Eyes found" << std::endl;
            break;
        }
        default:
        {
            EyeDetected eyes(detectedEyes);
            EyeDetectedEvent event(eyes);
            eyeDetectedDispacher.post(event);
            break;
        }
    }
    return detectedEyes;
}

DetectionOrchestrator::DetectionOrchestrator(const std::string& modelsPath)
{
    this->eyeCascadeDetector = std::make_shared<EyeDetectorCascade>(modelsPath + "/" + eyeDetectiongModel);
    this->faceCascadeDetector = std::make_shared<FaceDetectorCascade>(modelsPath + "/" + faceDetectionModel);
    this->blinkingLandmarksDetector = std::make_shared<EyeBlinkingDetector>(modelsPath + "/" + landmarksDetectionModel);
}

Dispatcher<BlinkDetected>& DetectionOrchestrator::getBlinkEventDispacher()
{
    return blinkDetectedDispacher;
}

Dispatcher<FaceDetected>& DetectionOrchestrator::getFaceEventDispacher()
{
    return faceDetectedDispacher;
}

Dispatcher<EyeDetected>& DetectionOrchestrator::getEyeEventDispacher()
{
    return eyeDetectedDispacher;
}