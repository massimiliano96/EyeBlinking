#include "EyeBlinkingDetector.hpp"
#include <list>

bool EyeBlinkingDetector::loadModels(std::string& models_path)
{
    if(!detector_eyes.load(models_path + "/haarcascade_eye_tree_eyeglasses.xml"))
    {
        std::cout << "LOAD CASCADE MODEL FAILED" << std::endl;
        return false;
    }
    dlib::deserialize(models_path + "/" +"shape_predictor_68_face_landmarks.dat") >> face_shape_predictor_;
    const std::string weights_filename = models_path + "/" + "yolov3-tiny_face-wider.weights";
    std::string config_filename  = models_path + "/" + "yolov3-tiny_face-wider.cfg";
    net_ = cv::dnn::readNetFromDarknet(config_filename, weights_filename);
    net_input_size_ = cv::Size(416, 416);
    net_input_scale_ = 1.0 / 255;
    return true;

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

void EyeBlinkingDetector::RatioBlink(const dlib::full_object_detection& shape)
{
    result_ratio = false;
    // check left eyes
    double P37_41_x = shape.part(37).x() - shape.part(41).x();
    double P37_41_y=  shape.part(37).y() -shape.part(41).y() ;
    double p37_41_sqrt=sqrt((P37_41_x * P37_41_x) + (P37_41_y * P37_41_y));

    double P38_40_x = shape.part(38).x() - shape.part(40).x();
    double P38_40_y = shape.part(38).y() - shape.part(40).y();
    double p38_40_sqrt=sqrt((P38_40_x * P38_40_x) + (P38_40_y * P38_40_y));

    double P36_39_x = shape.part(36).x() - shape.part(39).x();  
    double P36_39_y = shape.part(36).y() - shape.part(39).y();
    double p36_39_sqrt=sqrt((P36_39_x * P36_39_x) + (P36_39_y * P36_39_y));

    double EAR_left= (p37_41_sqrt +  p38_40_sqrt) / (2* p36_39_sqrt);

    // check right eyes
    double P43_47_x = shape.part(43).x() - shape.part(47).x();
    double P43_47_y=  shape.part(43).y() -shape.part(47).y() ;
    double p43_47_sqrt=sqrt((P43_47_x * P43_47_x) + (P43_47_y * P43_47_y));

    double P44_46_x = shape.part(44).x() - shape.part(46).x();
    double P44_46_y = shape.part(44).y() - shape.part(46).y();
    double p44_46_sqrt=sqrt((P44_46_x * P44_46_x) + (P44_46_y * P44_46_y));

    double P42_45_x = shape.part(42).x() - shape.part(45).x();  
    double P42_45_y = shape.part(42).y() - shape.part(45).y();
    double p42_45_sqrt=sqrt((P42_45_x * P42_45_x) + (P42_45_y * P42_45_y));

    double EAR_right= (p43_47_sqrt +  p44_46_sqrt) / (2* p42_45_sqrt);

    double EAR_total = ((EAR_right+EAR_left)/2);

    buffer_classes_eyes[buffer_count_eyes % buffer_size_eyes] = EAR_total;
    buffer_orizontal[buffer_count_eyes % buffer_size_eyes] = p36_39_sqrt;
    buffer_count_eyes++;
    
    if (buffer_count_eyes >= buffer_size_eyes)
    {
        if ((buffer_classes_eyes[1] <= (buffer_classes_eyes[0]*0.85) && (buffer_classes_eyes[1] <= (buffer_classes_eyes[2]*0.85)))
            || (buffer_classes_eyes[1] >= (buffer_classes_eyes[0]*1.15) && (buffer_classes_eyes[1] >= (buffer_classes_eyes[2]*1.15)))) 
        {
            std::cout<<"---------------------------"<<std::endl;
            std::cout<<"----------BLINK LANDMARK----------"<<std::endl;
            std::cout<<"---------------------------"<<std::endl;
            result_ratio = true;
        }
        else
        {
            result_ratio = false;
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

    //std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> rects;
    // region type
    if (net_out_layert_type_ == "DetectionOutput")
    {
        for (auto &mat : outs)
        {
            float *data = (float*)mat.data;
            for (size_t i = 0; i < mat.total(); i += 7)
            {
                float confidence = data[i + 2];
                if (confidence > confidence_threshold)
                {
                    int left   = (int)data[i + 3];
                    int top    = (int)data[i + 4];
                    int right  = (int)data[i + 5];
                    int bottom = (int)data[i + 6];
                    int width  = right - left + 1;
                    int height = bottom - top + 1;
                    
                    if ((width <= 2) || (height <= 2))
                    {
                        left   = (int)(data[i + 3] * mat_bgr.cols);
                        top    = (int)(data[i + 4] * mat_bgr.rows);
                        right  = (int)(data[i + 5] * mat_bgr.cols);
                        bottom = (int)(data[i + 6] * mat_bgr.rows);
                        width  = right - left + 1;
                        height = bottom - top + 1;
                    }
                    //const int class_id = (int)(data[i + 1]) - 1;
                    //std::cout << "class id " << class_id << std::endl;
                    //std::cout << "box " << left << " " << top << " " << width << " " << height << std::endl;
                    //class_ids.push_back (class_id);
                    confidences.push_back((float)confidence);
                    rects.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
    }
    else if (net_out_layert_type_ == "Region")
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

    
    cv::Rect rect_out = findFace(frame, confidence_threshold, nms_threshold, threshold_area);
   
    if(rect_out.empty())
        return;
    else
    {
        FaceDetected obj(rect_out);
        FaceDetectedEvent event(obj);
        face_dispacher.post(event);
    }

    dlib::rectangle rect = openCVRectToDlib(rect_out);

    IplImage ipl_img_bgr = cvIplImage(frame);
    dlib::cv_image<dlib::bgr_pixel> img(&ipl_img_bgr);

    dlib::full_object_detection shape = face_shape_predictor_(img, rect);

    CascadeBlink(frame, rect_out);
    RatioBlink(shape);

    if(result_cascade || result_ratio)
    {
        BlinkDetected obj;
        BlinkDetectedEvent event(obj);
        blink_dispacher.post(event);
    }
}