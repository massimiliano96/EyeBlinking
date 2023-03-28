#include "EyeBlinkingDetector.hpp"
#include "detection/impl/eyedetection/EyeDetectorCascade.hpp"
#include "detection/impl/facedetection/FaceDetectorCascade.hpp"
#include "events/EventType.hpp"
#include "events/FaceDetected.hpp"

const std::string faceDetectionModel = "haarcascade_frontalface_default.xml";
const std::string eyeDetectiongModel = "haarcascade_eye.xml";

void EyeBlinkingDetector::process(std::vector<cv::Mat>& frames)
{
    try
    {
        for (std::size_t i = 0; i < frames.size(); i++)
        {
            this->detectFace(frames[i]);
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

cv::Rect EyeBlinkingDetector::detectFace(cv::Mat& image)
{
    std::vector<cv::Rect> detectedFaces = faceDetector->detect(image);
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
            faceDetectedDispacher->post(event);
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

std::vector<cv::Rect> EyeBlinkingDetector::detectEyes(cv::Mat&)
{
    return std::vector<cv::Rect> {};
}

EyeBlinkingDetector::EyeBlinkingDetector(std::string modelsPath)
{
    this->faceDetector = std::make_shared<FaceDetectorCascade>(modelsPath + "/" + faceDetectionModel);
    this->eyeDetector = std::make_shared<EyeDetectorCascade>(modelsPath + "/" + eyeDetectiongModel);
}

const std::shared_ptr<Dispatcher<BlinkDetected>> EyeBlinkingDetector::getBlinkEventDispacher()
{
    return blinkDetectedDispacher;
}
const std::shared_ptr<Dispatcher<FaceDetected>> EyeBlinkingDetector::getFaceEventDispacher()
{
    return faceDetectedDispacher;
}