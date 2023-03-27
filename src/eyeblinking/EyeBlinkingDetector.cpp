#include "EyeBlinkingDetector.hpp"
#include <list>

bool EyeBlinkingDetector::loadModels(std::string& models_path)
{
    try
    {
        detector_eyes.load(models_path + "/haarcascade_eye_tree_eyeglasses.xml");
        dlib::deserialize(models_path + "/" +"shape_predictor_68_face_landmarks.dat") >> face_shape_predictor_;
        const std::string weights_filename = models_path + "/" + "yolov3-tiny_face-wider.weights";
        std::string config_filename  = models_path + "/" + "yolov3-tiny_face-wider.cfg";
        net_ = cv::dnn::readNetFromDarknet(config_filename, weights_filename);
        net_input_size_ = cv::Size(416, 416);
        net_input_scale_ = 1.0 / 255;
        return true;
    }
    catch(const cv::Exception & ex)
    {
        std::cout<<"LOAD MODEL FAILED"<<std::endl;
        std::cout<<ex.what()<<std::endl;
        return false;
    }
}

void EyeBlinkingDetector::CascadeBlink(cv::Mat mat, cv::Rect roi)
{
    result_cascade = false;
    if(buffer_cascade.size() == 3)
        buffer_cascade.pop_front();
 
    cv::Mat gray;
    cv::Size minimalObjectSizeEyes(30, 30);
    //cv::Size maximalObjectSizeEyes(50, 50);
    std::vector<cv::Rect> eyes;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);

    if(!roi.empty())
    {

        detector_eyes.detectMultiScale(gray(roi),eyes, 1.3, 5, 0 | 2, minimalObjectSizeEyes);
        if(eyes.size() > 0)
        {
            buffer_cascade.push_back(1);
        }
        else
        {
            buffer_cascade.push_back(0);
        }
    }
    else
        buffer_cascade.resize(0);

    if(buffer_cascade.size() == 3)
    {
        bool begin = *(buffer_cascade.begin());
        bool middle = *(buffer_cascade.begin()++);
        bool end = buffer_cascade.back();
        if(begin != middle || middle != end)
        {
            std::cout<<"---------------------------"<<std::endl;
            std::cout<<"----------BLINK CASCADE----------"<<std::endl;
            std::cout<<"---------------------------"<<std::endl;

            result_cascade = true;
            buffer_cascade.resize(0);
        }
    }  
}

cv::Rect EyeBlinkingDetector::findFace(cv::Mat mat_bgr, float confidence_threshold, float nms_threshold, float threshold_area)
{
    cv::Mat blob;
    cv::dnn::blobFromImage(mat_bgr, blob, net_input_scale_, net_input_size_, cv::Scalar(), true, false);

    // sets the input to the network
    net_.setInput(blob);

    // runs the forward pass to get output of the output layers
    std::vector<cv::Mat> outs;
    std::vector<cv::String> net_out_names_ = net_.getUnconnectedOutLayersNames();
    std::vector<int> net_out_layers_ = net_.getUnconnectedOutLayers();
    std::string net_out_layert_type_ = net_.getLayer(net_out_layers_[0])->type;

    net_.forward(outs, net_out_names_);

    std::vector<float> confidences;
    std::vector<cv::Rect> rects;
    // region type
    if (net_out_layert_type_ == "Region")
    {
        for (auto &mat : outs)
        {
            float *data = (float*)mat.data;
            for (int i = 0; i < mat.rows; ++i, data += mat.cols)
            {
                cv::Mat scores = mat.row(i).colRange(5, mat.cols);
                cv::Point class_id_point;
                double confidence;
                cv::minMaxLoc(scores, 0, &confidence, 0, &class_id_point);
                if (confidence > confidence_threshold)
                {
                    int centerX = (int)(data[0] * mat_bgr.cols);
                    int centerY = (int)(data[1] * mat_bgr.rows);
                    int width = (int)(data[2] * mat_bgr.cols);
                    int height = (int)(data[3] * mat_bgr.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    //class_ids.push_back(class_id_point.x);
                    confidences.push_back((float)confidence);
                    rects.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
    }

    cv::Rect rect_out;
    std::vector<int> indices;
    cv::dnn::NMSBoxes(rects, confidences, confidence_threshold, nms_threshold, indices);

    double confidence_max = 0.0;
    int idxMax = -1;

    int thresholdArea_mat_bgr = round((mat_bgr.cols*mat_bgr.rows)* threshold_area);
    for (auto idx : indices)
    {
        if(confidences[idx] >= confidence_max && ((rects[idx].width*rects[idx].height)>=thresholdArea_mat_bgr))
    	{
            confidence_max = confidences[idx];
            idxMax= idx;
        }
    }

    if(idxMax != -1)
        rect_out = rects[idxMax];
    
    return rect_out;
}

dlib::rectangle EyeBlinkingDetector::openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}

void EyeBlinkingDetector::detect(cv::Mat frame)
{
    float confidence_threshold = 0.8f;
    float nms_threshold = 0.4f; // non maximum suppression
    float threshold_area = 0.1;

    // search a face usign YOLO
    cv::Rect rect_out = findFace(frame, confidence_threshold, nms_threshold, threshold_area);
   
    if(rect_out.empty())
        return;
    else
    {
        // if a face is detected post the corresponding event
        FaceDetected obj(rect_out);
        FaceDetectedEvent event(obj);
        face_dispacher.post(event);
    }

    dlib::rectangle rect = openCVRectToDlib(rect_out);

    IplImage ipl_img_bgr = cvIplImage(frame);
    dlib::cv_image<dlib::bgr_pixel> img(&ipl_img_bgr);

    dlib::full_object_detection shape = face_shape_predictor_(img, rect);

    //CascadeBlink(frame, rect_out);
    RatioBlink(shape);

    // if a blink is detected post the event
    if(result_cascade || result_ratio)
    {
        BlinkDetected obj;
        BlinkDetectedEvent event(obj);
        blink_dispacher.post(event);
    }
}