#include "EyeDetectorCascade.hpp"

#include <opencv2/imgproc.hpp>

#include <iostream>

EyeDetectorCascade::EyeDetectorCascade(const std::string& modelFilename) : CascadeDetector()
{
    try
    {
        detector.load(modelFilename);
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("Failed to load Face Detector Cascade model");
    }
}

cv::Mat EyeDetectorCascade::preProcessImage(cv::Mat& image, cv::Rect faceRoi)
{
    try
    {
        cv::Mat roiImage = image.clone();
        return roiImage(faceRoi);
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error(e.what());
    }
}

std::vector<cv::Rect> EyeDetectorCascade::detect(cv::Mat& image, cv::Rect roi)
{
    try
    {
        cv::Mat preparedMat = preProcessImage(image, roi);
        std::vector<cv::Rect> detectedEyes;
        // Detect faces
        detector.detectMultiScale(preparedMat, detectedEyes);
        return detectedEyes;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error(e.what());
    }
}