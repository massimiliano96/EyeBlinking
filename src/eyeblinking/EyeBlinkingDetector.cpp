#include "EyeBlinkingDetector.hpp"
#include "detection/impl/eyedetection/EyeDetectorCascade.hpp"
#include "detection/impl/facedetection/FaceDetectorCascade.hpp"
#include "events/EventType.hpp"
#include "events/FaceDetected.hpp"

const std::string faceDetectionModel = "haarcascade_frontalface_default.xml";
const std::string eyeDetectiongModel = "haarcascade_eye.xml";

void EyeBlinkingDetector::process(cv::Mat& frame)
{
    try
    {        
        cv::Rect faceRoi = this->detectFace(frame);
        detectedEyes.emplace_back(this->detectEyes(frame, faceRoi));
        if(detectedEyes.size() >= 5)
        {
            bool isBlinkDetected = this->checkEyeBlink(detectedEyes);
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
    cv::Rect roi = cv::Rect(0, 0, image.cols, image.rows);
    cv::Mat preparedImage = faceDetector->preProcessImage(image, roi);
    std::vector<cv::Rect> detectedFaces = faceDetector->detect(preparedImage);
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
    cv::Mat preparedImage = eyeDetector->preProcessImage(image, faceRoi);
    std::vector<cv::Rect> detectedEyes = eyeDetector->detect(preparedImage);
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
    this->eyeDetector = std::make_shared<EyeDetectorCascade>(modelsPath + "/" + eyeDetectiongModel);
    this->faceDetector = std::make_shared<FaceDetectorCascade>(modelsPath + "/" + faceDetectionModel);
}

Dispatcher<BlinkDetected>& EyeBlinkingDetector::getBlinkEventDispacher() 
{
    return blinkDetectedDispacher;
}
Dispatcher<FaceDetected>& EyeBlinkingDetector::getFaceEventDispacher()  
{
    return faceDetectedDispacher;
}