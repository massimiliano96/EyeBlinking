#include "EyeDetectorCascade.hpp"

#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <iostream>

EyeDetectorCascade::EyeDetectorCascade(const std::string& modelFilename) : CascadeDetector()
{
    try
    {
        if (std::filesystem::exists(modelFilename))
        {
            std::cout << "File exists!" << std::endl;
        }
        else
        {
            std::cout << "File does not exist." << std::endl;
            throw std::runtime_error("Model does not exist");
        }
        detector.load(modelFilename);
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("Failed to load Eye CascadeDetector Cascade model");
    }
}

cv::Mat EyeDetectorCascade::preProcess(cv::Mat& image, cv::Rect& faceRoi)
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

std::vector<cv::Rect> EyeDetectorCascade::postProcess(std::vector<cv::Rect>& detetedEyes, cv::Rect& faceRoi)
{
    try
    {
        std::vector<cv::Rect> eyes = detetedEyes;
        for (cv::Rect& eye : eyes)
        {
            eye.x = eye.x + faceRoi.x;
            eye.y = eye.y + faceRoi.y;
        }
        return eyes;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error(e.what());
    }
}

std::vector<cv::Rect> EyeDetectorCascade::detect(cv::Mat& image)
{
    try
    {
        std::vector<cv::Rect> detectedEyes;
        // Detect faces
        detector.detectMultiScale(image, detectedEyes, 1.1, 3, 0, cv::Size(20, 20));
        return detectedEyes;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error(e.what());
    }
}