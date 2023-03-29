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
        std::list<std::vector<cv::Rect>> eyes;
        for (std::size_t i = 0; i < frames.size(); i++)
        {
            cv::Rect faceRoi = this->detectFace(frames[i]);
            std::vector<cv::Rect> currentEyes = this->detectEyes(frames[i], faceRoi);
            eyes.emplace_back(currentEyes);
        }
        bool isBlinkDetected = this->checkEyeBlink(eyes);
        if (isBlinkDetected)
        {
            BlinkDetected obj;
            BlinkDetectedEvent event(obj);
            this->blinkDetectedDispacher->post(event);
        }
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

bool EyeBlinkingDetector::checkEyeBlink(std::list<std::vector<cv::Rect>>& eyes)
{
    int sizeChanges = 0;
    int lastSize = -1;

    for (const auto& vec : eyes)
    {
        if (lastSize != -1 && vec.size() != lastSize)
        {
            sizeChanges++;
        }
        lastSize = vec.size();
    }

    return sizeChanges > 1;
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

std::vector<cv::Rect> EyeBlinkingDetector::detectEyes(cv::Mat& image, cv::Rect& faceRoi)
{
    std::vector<cv::Rect> detectedEyes = eyeDetector->detect(image, faceRoi);
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