#include <opencv2/imgcodecs.hpp>
#include "eyeblinking/detection/cascade/impl/facedetection/FaceDetectorCascade.hpp"
#include "gtest/gtest.h"

class FaceDetectorCascadeTest : public testing::Test
{
public:
    // Test data
    std::string modelFilename = "models/haarcascade_frontalface_default.xml";
    cv::Mat image = cv::imread("testData/TestImg.jpg");
};

TEST_F(FaceDetectorCascadeTest, ModelFileExists)
{
    // Test that the model file exists
    ASSERT_NO_THROW(FaceDetectorCascade detector(modelFilename));
}

TEST_F(FaceDetectorCascadeTest, ConstructorThrowsWhenModelFileDoesNotExist)
{
    EXPECT_THROW(FaceDetectorCascade detector("invalid_model_file.xml"), std::runtime_error);
}

TEST_F(FaceDetectorCascadeTest, DetectReturnsNonEmptyVectorWhenFacesAreDetected)
{
    FaceDetectorCascade detector(modelFilename);
    std::vector<cv::Rect> detectedFaces = detector.detect(image);
    ASSERT_FALSE(detectedFaces.empty());
}

TEST_F(FaceDetectorCascadeTest, DetectReturnsEmptyVectorWhenNoFacesAreDetected)
{
    cv::Mat testImage(200, 200, CV_8UC1, cv::Scalar(0));
    FaceDetectorCascade detector(modelFilename);
    std::vector<cv::Rect> detectedFaces = detector.detect(testImage);
    ASSERT_TRUE(detectedFaces.empty());
}

TEST_F(FaceDetectorCascadeTest, PreProcessReturnsGrayScaleImage)
{
    FaceDetectorCascade detector(modelFilename);
    cv::Rect roi(0, 0, image.cols, image.rows);
    cv::Mat processedImage = detector.preProcess(image, roi);
    ASSERT_EQ(processedImage.channels(), 1);
}

TEST_F(FaceDetectorCascadeTest, PostProcessReturnsInputVectorUnchanged)
{
    std::vector<cv::Rect> detectedFaces = {cv::Rect(0, 0, 100, 100), cv::Rect(150, 150, 100, 100)};
    cv::Rect imageRect(0, 0, 500, 500);
    FaceDetectorCascade detector(modelFilename);
    std::vector<cv::Rect> processedFaces = detector.postProcess(detectedFaces, imageRect);
    ASSERT_EQ(processedFaces.size(), detectedFaces.size());
    for (int i = 0; i < processedFaces.size(); i++)
    {
        ASSERT_EQ(processedFaces[i], detectedFaces[i]);
    }
}
