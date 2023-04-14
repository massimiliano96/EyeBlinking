#ifndef _CASCADE_DETECTOR_HPP_
#define _CASCADE_DETECTOR_HPP_

#include <opencv2/core.hpp>

class CascadeDetector
{
public:
    CascadeDetector() = default;

    virtual ~CascadeDetector() = default;

    virtual std::vector<cv::Rect> detect(cv::Mat& image) = 0;

    virtual cv::Mat preProcess(cv::Mat&, cv::Rect&) = 0;

    virtual std::vector<cv::Rect> postProcess(std::vector<cv::Rect>&, cv::Rect&) = 0;
};

#endif