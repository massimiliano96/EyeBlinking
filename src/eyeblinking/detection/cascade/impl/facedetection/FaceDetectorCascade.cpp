#include "FaceDetectorCascade.hpp"

#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <iostream>

FaceDetectorCascade::FaceDetectorCascade(const std::string& modelFilename) : CascadeDetector()
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
        throw std::runtime_error("Failed to load Face CascadeDetector Cascade model");
    }
}

cv::Mat FaceDetectorCascade::preProcess(cv::Mat& image, cv::Rect& roi)
{
    try
    {
        cv::Mat grayImage;
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(grayImage, grayImage);
        return grayImage;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}

std::vector<cv::Rect> FaceDetectorCascade::postProcess(std::vector<cv::Rect>& detectedFaces, cv::Rect& imageRect)
{
    // NO OP
    return detectedFaces;
}

std::vector<cv::Rect> FaceDetectorCascade::detect(cv::Mat& image)
{
    try
    {
        std::vector<cv::Rect> detectedFaces;
        // Detect faces
        detector.detectMultiScale(image, detectedFaces, 1.1, 3, 0, cv::Size(60, 60));
        return detectedFaces;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}