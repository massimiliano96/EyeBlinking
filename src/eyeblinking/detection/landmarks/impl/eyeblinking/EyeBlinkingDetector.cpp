#include "EyeBlinkingDetector.hpp"

#include <dlib/opencv.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs.hpp>

bool EyeBlinkingDetector::detect(dlib::full_object_detection& shape)
{
    std::cout << "Blink Detection starts" << std::endl;
    Eye leftEye;
    leftEye.pointL = shape.part(36);
    leftEye.pointLT = shape.part(37);
    leftEye.pointRT = shape.part(38);
    leftEye.pointR = shape.part(39);
    leftEye.pointRB = shape.part(40);
    leftEye.pointLB = shape.part(41);
    bool leftBlink = this->checkEyeBlink(leftEye, this->ratiosBufferLeft);

    Eye rightEye;
    rightEye.pointL = shape.part(42);
    rightEye.pointLT = shape.part(43);
    rightEye.pointRT = shape.part(44);
    rightEye.pointR = shape.part(45);
    rightEye.pointRB = shape.part(46);
    rightEye.pointLB = shape.part(47);
    bool rightBlink = this->checkEyeBlink(rightEye, this->ratiosBufferRight);

    return (leftBlink || rightBlink);
}

bool EyeBlinkingDetector::checkEyeBlink(Eye& eye, FixedSizeQueue<double, ratioBufferSize>& buffer) const
{
    double distanceL = getDistance(eye.pointLT, eye.pointLB);
    double distanceR = getDistance(eye.pointRT, eye.pointRB);
    double distanceH = getDistance(eye.pointL, eye.pointR);
    double ratio = (distanceL + distanceR) / distanceH;
    buffer.push(ratio);
    if (buffer.size() >= ratioBufferSize)
    {
        double variability = buffer.variability();
        std::cout << "Varibility = " << variability << std::endl;
        if (variability >= 0.01)
        {
            buffer.clear();
            return true;
        }
    }
    return false;
}

dlib::full_object_detection EyeBlinkingDetector::preProcess(cv::Mat& image, cv::Rect& roi)
{
    IplImage iplImg = cvIplImage(image);
    dlib::cv_image<dlib::bgr_pixel> dlibColorImg(&iplImg);
    dlib::matrix<dlib::rgb_pixel> dlibMatrix;
    dlib::assign_image(dlibMatrix, dlibColorImg);
    const dlib::rectangle& faceRect = dlib::rectangle(roi.x, roi.y, roi.x + roi.width, roi.y + roi.height);
    dlib::full_object_detection shape = detector(dlibMatrix, faceRect);
    return shape;
}

double EyeBlinkingDetector::getDistance(dlib::point& pointA, dlib::point& pointB) const
{
    double distanceX = pointA.x() - pointB.x();
    double distanceY = pointA.y() - pointB.y();
    return std::sqrt(std::pow(distanceX, 2) + std::pow(distanceY, 2));
}

EyeBlinkingDetector::EyeBlinkingDetector(const std::string& modelFilename) : LandmarksDetector()
{
    dlib::deserialize(modelFilename) >> detector;
}