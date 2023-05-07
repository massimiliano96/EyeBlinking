#include <gtest/gtest.h>
#include <opencv2/imgcodecs.hpp>
#include "eyeblinking/detection/cascade/impl/eyedetection/EyeDetectorCascade.hpp"

class EyeDetectorCascadeTest : public testing::Test
{
public:
    // Test data
    std::string modelFilename = "models/haarcascade_eye.xml";
    cv::Mat image = cv::imread("tests/testData/TestImg.jpg");
};

TEST_F(EyeDetectorCascadeTest, ModelFileExists)
{
    // Test that the model file exists
    ASSERT_NO_THROW(EyeDetectorCascade detector(modelFilename));
}

TEST_F(EyeDetectorCascadeTest, ConstructorThrowsWhenModelFileDoesNotExist)
{
    ASSERT_THROW(EyeDetectorCascade detector("invalid_model_file.xml"), std::runtime_error);
}

TEST_F(EyeDetectorCascadeTest, DetectEyesInImage)
{
    // Test that eyes can be detected in an image
    EyeDetectorCascade detector(modelFilename);
    std::vector<cv::Rect> detectedEyes = detector.detect(image);
    ASSERT_GT(detectedEyes.size(), 0);
}

TEST_F(EyeDetectorCascadeTest, DetectEyesInBlackImage)
{
    // Test that eyes can be detected in an image
    EyeDetectorCascade detector(modelFilename);
    cv::Mat testImage(200, 200, CV_8UC1, cv::Scalar(0));
    std::vector<cv::Rect> detectedEyes = detector.detect(testImage);
    ASSERT_EQ(detectedEyes.size(), 0);
}

TEST_F(EyeDetectorCascadeTest, PreProcessImage)
{
    // Test that the image can be preprocessed
    EyeDetectorCascade detector(modelFilename);
    cv::Rect faceRoi(0, 0, 100, 100);
    cv::Mat eyeImage = detector.preProcess(image, faceRoi);
    ASSERT_EQ(eyeImage.cols, faceRoi.width);
    ASSERT_EQ(eyeImage.rows, faceRoi.height);
}

TEST_F(EyeDetectorCascadeTest, PostProcessEyes)
{
    // Test that detected eyes can be postprocessed
    EyeDetectorCascade detector(modelFilename);
    std::vector<cv::Rect> detectedEyes = {cv::Rect(0, 0, 10, 10), cv::Rect(10, 10, 30, 30)};
    cv::Rect faceRoi(20, 20, 100, 100);
    std::vector<cv::Rect> postProcessedEyes = detector.postProcess(detectedEyes, faceRoi);
    ASSERT_EQ(postProcessedEyes.size(), detectedEyes.size());
    for (int i = 0; i < detectedEyes.size(); i++)
    {
        ASSERT_EQ(postProcessedEyes[i].x, detectedEyes[i].x + faceRoi.x);
        ASSERT_EQ(postProcessedEyes[i].y, detectedEyes[i].y + faceRoi.y);
        ASSERT_EQ(postProcessedEyes[i].width, detectedEyes[i].width);
        ASSERT_EQ(postProcessedEyes[i].height, detectedEyes[i].height);
    }
}
