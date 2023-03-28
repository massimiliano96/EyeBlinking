#include "FaceDetectorCascade.hpp"

#include <opencv2/imgproc.hpp>

#include <iostream>

FaceDetectorCascade::FaceDetectorCascade(const std::string& modelFilename) : CascadeDetector()
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

cv::Mat FaceDetectorCascade::preProcessImage(cv::Mat& image)
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

std::vector<cv::Rect> FaceDetectorCascade::detect(cv::Mat& image, cv::Rect roi)
{
    try
    {
        cv::Mat preparedMat = preProcessImage(image);
        std::vector<cv::Rect> detectedFaces;
        // Detect faces
        detector.detectMultiScale(preparedMat, detectedFaces);
        return detectedFaces;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error(e.what());
    }
}