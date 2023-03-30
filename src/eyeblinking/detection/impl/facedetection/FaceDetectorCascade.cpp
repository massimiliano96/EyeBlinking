#include "FaceDetectorCascade.hpp"

#include <opencv2/imgproc.hpp>

#include <iostream>
#include <filesystem>

FaceDetectorCascade::FaceDetectorCascade(const std::string& modelFilename) : CascadeDetector()
{
    try
    {
        if (std::filesystem::exists(modelFilename)) {
            std::cout << "File exists!" << std::endl;
        } else {
            std::cout << "File does not exist." << std::endl;
        }
        detector.load(modelFilename);
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("Failed to load Face Detector Cascade model");
    }
}

cv::Mat FaceDetectorCascade::preProcessImage(cv::Mat& image, cv::Rect& roi)
{
    try
    {
        cv::Mat grayImage;
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(grayImage, grayImage);
        return grayImage(roi);
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}

std::vector<cv::Rect> FaceDetectorCascade::detect(cv::Mat& image)
{
    try
    {
        std::vector<cv::Rect> detectedFaces;
        // Detect faces
        detector.detectMultiScale(image, detectedFaces);
        return detectedFaces;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}