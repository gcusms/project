#include "rm_armorfitted.h"

extern const int RED_ARMOR_GRAY_TH;
extern const int RED_ARMOR_COLOR_TH;
extern const int BLUE_ARMOR_GRAY_TH;
extern const int BLUE_ARMOR_COLOR_TH;

/**
 * @brief 拟合装甲板类
 * 
 * @param light_left 装甲板左边灯条
 * @param light_right 装甲板左边灯条
 * @param angle_solve 角度解算对象
 * @param roi ROI对象
 */
void CandidateArmor::inputParam(NiceLight &light_left, NiceLight &light_right, RM_SolveAngle &angle_solve, ArmorROI &roi){
    rect = fit_Rrect(light_left.rect, light_right.rect);             
    
    armor_width = centerDistance(light_left.rect.center, light_right.rect.center);
    convert_center = rect.center + Point2f(roi.tl);//set center of roi
    dist_to_center = centerDistance(convert_center,IMG_CENTER);//未用上
    
    rect = RotatedRect(convert_center,rect.size,rect.angle);                          
    float _w = float(sqrt(pow(fabs(light_left.depth - light_right.depth), 2) + pow(armor_width, 2)));
    //RotatedRect _rect = RotatedRect(rect.center,Size(_w,rect.size.height),rect.angle);
    float temp_w = rect.size.width;
    // rect.size.width = _w;
    angle_solve.run_SolvePnp(rect, SMALL_ARMOR_SIZE_W, SMALL_ARMOR_SIZE_H);
    rect.size.width = temp_w;                      

    depth = int(angle_solve.dist);
}

/**
 * @brief 将左右灯条拟合为一个旋转矩形
 *
 * @param rect_left 左边灯条的旋转矩形
 * @param rect_right 右边灯条的旋转矩形
 * @return RotatedRect
 */
RotatedRect CandidateArmor::fit_Rrect(RotatedRect &rect_left,RotatedRect &rect_right){
    // if(rect_left.size.width<0){
    //     rect_left.size.width = 0;
    // }
    // if(rect_right.size.width<0){
    //     rect_right.size.width = 0;
    // }
    
    Point2f center = (rect_left.center+rect_right.center)*0.5;
    float center_slope = (rect_left.center.y-rect_right.center.y)/(rect_left.center.x-rect_right.center.x);
    float distance = centerDistance(rect_left.center,rect_right.center);
    float width_left =  MIN(rect_left.size.width,rect_left.size.height);
    float height_left = MAX(rect_left.size.width,rect_left.size.height);
    float width_right = MIN(rect_right.size.width,rect_right.size.height);
    float height_right =MAX(rect_right.size.width,rect_right.size.height);

    float W = distance +(width_left/2 + width_right/2);//roi of  rect.width
    if(W < 0){
        W = 0;
    }
    float H = MAX(height_left,height_right);//*2.27;
    // cout<<"rect_left.size.width=="<<W<<endl;//test
    // cout<<"rect_right.size.width=="<<H<<endl;
    float angle = atan(center_slope);
    //float angle = atan2((rect_left.center.y-rect_right.center.y),(rect_left.center.x-rect_right.center.x));
    RotatedRect Rrect = RotatedRect(center,Size2f(W,H),angle*180/float(CV_PI));
    // -90~0, 0~90 (minArearent)  0~180 (fitEllipse)
    return Rrect;
}

/**
 * @brief Construct a new rm armorfitted::rm armorfitted object
 *
 * @note 如果 IS_NUMBER_PREDICT_OPEN 宏定义使能，则会导入tensorflow识别手写数字的模型
 * 并对装甲板上的数字进行识别，作为自动打击的一个判断条件
 */
RM_ArmorFitted::RM_ArmorFitted(){
    #if IS_NUMBER_PREDICT_OPEN == 1
    //Set GPU options
    opts.config.mutable_gpu_options() -> set_allow_growth(true);
    status = NewSession(opts, &session);//创建新会话Session
    /*------------读取模型------------*/
    status_load = tensorflow::ReadBinaryProto(Env::Default(), PB_MODEL_PATH, &graphdef);//从pb文件中读取模型
    if(!status_load.ok()) {
        cout<< "ERROR:Loading model failed..."<< PB_MODEL_PATH <<endl;
        cout<< status_load.ToString() << endl;
        read_model_success = false;
    }
    status_create = session -> Create(graphdef);//将模型导入Session中
    if (!status_create.ok()) {
        cout << "ERROR: Creating graph in session failed..." << status_create.ToString() << std::endl;
        read_model_success = false;
    }
    if(read_model_success){
        cout << "<----Successfully created session and load graph.------->"<< endl;
        cout << "<----成功创建会话并导入模型------->"<< endl;
    }
    #endif
}

/**
 * @brief Destroy the rm armorfitted::rm armorfitted object
 *
 */
RM_ArmorFitted::~RM_ArmorFitted(){
}

/**
 * @brief 装甲识别预处理函数
 *
 * @param frame 相机捕获的初始图像
 * @note 预处理部分占用程序大部分效率，并且影响之后处理结果，应当作为主要优化方向
 */
void RM_ArmorFitted::imageProcessing(Mat frame, int my_color){
    src_img = frame;
    #if ENABLE_ROI_RECT == 1 && SHOW_BIN_IMG == 0
    if(is_last_data_catch && !is_Lost_target){
        roi.updateROI(src_img,is_Lost_target,lose_target_cnt,last_armor,armor_depth);
        roi_img = roi.ROI_img;
    }else if(!is_Lost_target && !is_last_data_catch && lose_target_cnt <= LOSE_CNT_MAX){
        roi.updateROI(src_img,is_Lost_target,lose_target_cnt,last_armor,armor_depth);
        roi_img = roi.ROI_img;
        lose_target_cnt += 1;
    }else {
        lose_target_cnt = 0;
        is_Lost_target = true;
        armor_depth = 0;
        roi.updateROI(src_img,is_Lost_target,lose_target_cnt,last_armor,armor_depth);
        roi_img = roi.ROI_img;
    }
    #else
    roi_img = src_img;
    #endif
    src_img.copyTo(dst_img);
    /* 获取灰度图 */
    imshow("roi_img", roi_img);
    cvtColor(roi_img, gray_img, COLOR_BGR2GRAY);
    //cvtColor(roi_img, hsv_img, COLOR_BGR2HSV);
    vector<Mat> _split;     //B G R
    split(roi_img,_split);

    switch (my_color) {
    case RED:
    {
        /* my_color为红色，则处理蓝色的情况 */
        /* 灰度图与RGB同样做蓝色处理 */
        subtract(_split[0], _split[2], bin_img_color);// b - r
        #if IS_PARAM_ADJUSTMENT == 1
        Mat trackbar_img = Mat::zeros(1,1200,CV_8UC1);
        namedWindow("trackbar");
        cv::createTrackbar("GRAY_TH_BLUE:", "trackbar", &GRAY_TH_BLUE, 255, nullptr);
        cv::createTrackbar("COLOR_TH_BLUE:", "trackbar", &COLOR_TH_BLUE, 255, nullptr);
        threshold(gray_img, bin_img_gray, GRAY_TH_BLUE, 255, THRESH_BINARY);
        threshold(bin_img_color, bin_img_color, COLOR_TH_BLUE, 255, THRESH_BINARY);
        imshow("trackbar",trackbar_img);
        #else
        threshold(gray_img, bin_img_gray, BLUE_ARMOR_GRAY_TH, 255, THRESH_BINARY);
        threshold(bin_img_color, bin_img_color, BLUE_ARMOR_COLOR_TH, 255, THRESH_BINARY);
        #endif

    } break;
    case BLUE:
    {
        /* my_color为蓝色，则处理红色的情况 */
        /* 灰度图与RGB同样做红色处理 */
        subtract(_split[2], _split[0], bin_img_color);// r - b
        #if IS_PARAM_ADJUSTMENT == 1
        Mat trackbar_img = Mat::zeros(1,1200,CV_8UC1);
        namedWindow("trackbar");
        cv::createTrackbar("GRAY_TH_RED:", "trackbar", &GRAY_TH_RED, 255, nullptr);
        cv::createTrackbar("COLOR_TH_RED:", "trackbar", &COLOR_TH_RED, 255, nullptr);
        threshold(gray_img, bin_img_gray, GRAY_TH_RED, 255, THRESH_BINARY);
        threshold(bin_img_color, bin_img_color, COLOR_TH_RED, 255, THRESH_BINARY);
        imshow("trackbar",trackbar_img);
        #else
        threshold(gray_img, bin_img_gray, RED_ARMOR_GRAY_TH, 255, THRESH_BINARY);
        threshold(bin_img_color, bin_img_color, RED_ARMOR_COLOR_TH, 255, THRESH_BINARY);
        #endif

    } break;
    default:
    {
        /* my_color为ALL_COLOR，则包括两种情况 */
        Mat bin_img_color_1;
        Mat bin_img_color_2;
        subtract(_split[0], _split[2], bin_img_color_1);// b - r
        subtract(_split[2], _split[0], bin_img_color_2);// r - b
        #if IS_PARAM_ADJUSTMENT == 1
        Mat trackbar_img = Mat::zeros(1,1200,CV_8UC1);
        namedWindow("trackbar");
        cv::createTrackbar("GRAY_TH_RED:", "trackbar", &GRAY_TH_RED, 255, nullptr);
        cv::createTrackbar("GRAY_TH_BLUE:", "trackbar", &GRAY_TH_BLUE, 255, nullptr);
        cv::createTrackbar("COLOR_TH_BLUE:", "trackbar", &COLOR_TH_BLUE, 255, nullptr);
        cv::createTrackbar("COLOR_TH_RED:", "trackbar", &COLOR_TH_RED, 255, nullptr);
        int th = int((GRAY_TH_RED +GRAY_TH_BLUE)*0.5);
        threshold(gray_img, bin_img_gray, th, 255, THRESH_BINARY);
        threshold(bin_img_color_1, bin_img_color_1, COLOR_TH_BLUE, 255, THRESH_BINARY);
        threshold(bin_img_color_2, bin_img_color_2, COLOR_TH_RED, 255, THRESH_BINARY);
        imshow("trackbar",trackbar_img);
        #else
        int th = int((BLUE_ARMOR_GRAY_TH + RED_ARMOR_GRAY_TH)*0.5);
        threshold(gray_img, bin_img_gray, th, 255, THRESH_BINARY);
        threshold(bin_img_color_1, bin_img_color_1, BLUE_ARMOR_COLOR_TH, 255, THRESH_BINARY);
        threshold(bin_img_color_2, bin_img_color_2, RED_ARMOR_COLOR_TH, 255, THRESH_BINARY);
        #endif
        bitwise_or(bin_img_color_1,bin_img_color_2,bin_img_color);// 求并集
    } break;
    }

    Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,7));//MORPH_ELLIPSE,cv::Size(3,7)
    dilate(bin_img_gray,bin_img_gray,element);
    medianBlur(bin_img_color,bin_img_color,5);
    dilate(bin_img_color,bin_img_color,element);
    //medianBlur(bin_img_hsv,bin_img_hsv,3);
    /* 图像预处理结束，最终得到两张二值图像 bin_img_gray bin_img_hsv */
    #if SHOW_BIN_IMG == 1
    imshow("bin_img_color",bin_img_color);
    imshow("bin_img_gray", bin_img_gray);
    #endif
    bitwise_and(bin_img_color, bin_img_gray, bin_img_color);
    #if SHOW_BIN_IMG == 1
    imshow("bin_img_final", bin_img_color);
    #endif

    _split.clear();
    vector<Mat>(_split).swap(_split);
} 

/**
 * @brief 装甲识别执行函数
 *
 */
void RM_ArmorFitted::armorFitted(){
    is_last_data_catch = false;
    float rect_area;
    
    vector <NiceLight> candidate_lights;   //符合条件的灯条实例化

    vector < vector < Point > > contours;
    vector < Vec4i > hierarchy;
    findContours(bin_img_color, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
    for (size_t j = 0; j < contours.size();++j){
        if (contours[j].size() < 6){continue;}
        RotatedRect R_rect = fitEllipse(contours[j]);
        float width = MIN(R_rect.size.width,R_rect.size.height);
        float height =MAX(R_rect.size.width,R_rect.size.height);
        rect_area = width * height;
        float w_h_ratio = width / height;
        if ((w_h_ratio < 0.4f) /*高宽比,角度筛选形状符合要求的灯条*/
                && ((0<= R_rect.angle && R_rect.angle<=45)||(135<=R_rect.angle && R_rect.angle<=180)) && rect_area > 50 ){
            light.inputParam(R_rect, angle_solve);
            candidate_lights.push_back(light);
            #if DRAW_LIGHT == 1
            Point2f vtx[4];
            R_rect.points(vtx);
            for (int j = 0; j < 4; j++){line(dst_img, vtx[j] + (Point2f)roi.tl, vtx[(j + 1) % 4] + (Point2f)roi.tl, Scalar(200, 0, 0),2,8,0);}
            Point put_Point_R_rect_angle = Point(R_rect.center.x + roi.tl.x,R_rect.center.y + roi.tl.y);
            putText(dst_img, to_string(R_rect.angle),put_Point_R_rect_angle,FONT_HERSHEY_PLAIN,1,Scalar(150, 100, 0),1,8,false);
            Point put_h = Point(R_rect.center.x + roi.tl.x,R_rect.center.y + roi.tl.y + 10);
            putText(dst_img, to_string(R_rect.size.height),put_h,FONT_HERSHEY_PLAIN,1,Scalar(100, 0, 100),1,8,false);
            Point put_w = Point(R_rect.center.x + roi.tl.x,R_rect.center.y + roi.tl.y + 20);
            putText(dst_img, to_string(R_rect.size.width),put_w,FONT_HERSHEY_PLAIN,1,Scalar(100, 200, 0),1,8,false);
            #endif
        }
    }//筛选灯条循环结束

    #if SHOW_DEBUG_INFORMATION == 1
    float max_dis_th;
    float light_distance_val;
    float light_dis_equ_maxh;
    float w1_equ_w2;
    #endif
    
    float light_addH_max = 0;
    RotatedRect armor_rect;
    for (size_t i = 0;i<candidate_lights.size();++i){
        for (size_t j = i+1;j<candidate_lights.size();++j){
            //区别出左右灯条
            if(candidate_lights[i].rect.center.x < candidate_lights[j].rect.center.x){
                left_light = candidate_lights[i];
                right_light = candidate_lights[j];
            } else {
                left_light = candidate_lights[j];
                right_light = candidate_lights[i];
            }
            //灯条的角度
            float angle_left = left_light.rect.angle;
            float angle_right = right_light.rect.angle;
            //灯条的宽高
            float w1 = MIN(left_light.rect.size.width,left_light.rect.size.height);
            float h1 = MAX(left_light.rect.size.width,left_light.rect.size.height);
            float w2 = MIN(right_light.rect.size.width,right_light.rect.size.height);
            float h2 = MAX(right_light.rect.size.width,right_light.rect.size.height);
            //灯条高度差
            float light_y_diff = fabs(left_light.rect.center.y - right_light.rect.center.y);
            bool is_height_diff_catch = (light_y_diff < (h1+h2)*0.5f);

            //灯条间的距离
            float light_distance = centerDistance(left_light.rect.center, right_light.rect.center);
            bool is_light_distance_catch = (light_distance < MAX(h1,h2) * 5);

            //灯条之间角度的关系
            bool is_light_angle_catch = false;
            if((angle_left <90.f && angle_right < 90.f) || (angle_left > 90.f && angle_right > 90.f)){
                //同侧
                is_light_angle_catch = (fabs(angle_left-angle_right) <= 10.f);
            } else if(angle_left < 90.f && angle_right > 90.f){
                //--- \ / --- 内八
                is_light_angle_catch = (170.f <= fabs(angle_left-angle_right) && fabs(angle_left-angle_right) <= 180.f);
            } else if(angle_left > 90.f && angle_right < 90.f){
                //--- / \ --- 外八
                is_light_angle_catch = (170.f <= fabs(angle_left-angle_right) && fabs(angle_left-angle_right) <= 180.f);
            } else if(angle_left == 0.f && angle_right != 0.f){
                //左边竖直
                is_light_angle_catch = (170.f <= fabs(angle_left-angle_right) || fabs(angle_left-angle_right) <= 10.f);
            } else if(angle_left != 0.f && angle_right == 0.f){
                //右边竖直
                is_light_angle_catch = (170.f <= fabs(angle_left-angle_right) || fabs(angle_left-angle_right) <= 10.f);
            }

            is_light_distance_catch = true;

            if(is_height_diff_catch
                    && is_light_angle_catch
                    && is_light_distance_catch)
            {
                is_last_data_catch = true;//检测到装甲板，则下一帧会标识上一帧有数据
                is_Lost_target = false;//未丢失目标
                lose_target_cnt = 0;//计数器置0
                if(h1 + h2 >= light_addH_max){
                    light_addH_max = h1 + h2;
                    armor.inputParam(left_light, right_light, angle_solve, roi);

                    #if SHOW_DEBUG_INFORMATION == 1
                    light_distance_val = light_distance;
                    max_dis_th = MAX(h1,h2) * 5;
                    light_dis_equ_maxh = light_distance/MAX(h1,h2);
                    w1_equ_w2 = MIN(w1,w2)/MAX(w1,w2);
                    #endif
                } //获取所有配对装甲中灯条高度加起来最大的作为)最优装甲
            }
        }
    }//匹配灯条循环结束

    //角度解算,获得最终发送给云台的数据
    int shooting = 0;
    if(is_last_data_catch){
        armor_rect = armor.rect;
        last_armor = armor_rect;
        roi.setNumROI(src_img, armor_rect);
        float armor_aspect_ratio = MIN(armor_rect.size.width,armor_rect.size.height)/MIN(armor_rect.size.width,armor_rect.size.height);
        // 数字识别
        #if IS_NUMBER_PREDICT_OPEN == 1
        int output_class_id = getLabel(roi.ROI_armor_ID);   //获取装甲板ID
        #endif
        // 数字识别

        #if SHOW_DEBUG_INFORMATION == 1
        putText(dst_img,to_string(armor_aspect_ratio),armor_rect.center,FONT_HERSHEY_PLAIN,2,Scalar(150, 200, 0),1,8,false);  // 装甲宽高比
        #endif

        #if DRAW_ARMOR_RECT == 1
        Point2f vtx[4];
        armor_rect.points(vtx);
        for (int j = 0; j < 4; j++){
            line(dst_img, vtx[j], vtx[(j + 1) % 4], Scalar(150, 200, 0),2,8,0); //装甲板rect
        }
        #endif
        
        #if IS_KF_PREDICT_ARMOR_OPEN == 1
        Point2f predict_p = kf.predict_point(_t,armor_rect.center);
        RotatedRect predict_armor_rect = RotatedRect(predict_p, armor_rect.size, armor_rect.angle);
        kf_reset_cnt = 0;
        Point2f vtx_p[4];
        predict_armor_rect.points(vtx_p);
        for (int j = 0; j < 4; j++){
            line(dst_img, vtx_p[j], vtx_p[(j + 1) % 4], Scalar(150, 0, 200),2,8,0);
        }
        #endif
        
        #if SERIAL_COMMUNICATION_PLAN == 0
        /* 二维＋深度 */
        yaw_data = armor_rect.center.x;
        pitch_data = armor_rect.center.y;
        _yaw_data = 0;
        _pitch_data = 0;

        #else
        /* Angle */
        yaw_data = fabs(angle_solve.angle_x)*1000;
        pitch_data = fabs(angle_solve.angle_y)*1000;
        _yaw_data = (yaw_data >=0 ? 0:1);
        _pitch_data = (pitch_data <=0 ? 0:1);
        #endif
        
    } else {
        kf_reset_cnt += 1;
        #if SERIAL_COMMUNICATION_PLAN == 0
        /* 二维＋深度 */
        yaw_data = int(src_img.cols*0.5);
        pitch_data = int(src_img.rows*0.5);
        #else
        /* Angle */
        yaw_data = 0.0;
        pitch_data = 0.0;
        #endif
        _yaw_data = 0;
        _pitch_data = 0;
    }
    #if SHOW_DEBUG_INFORMATION == 1
    Point p = Point(armor_rect.center.x,armor_rect.center.y+20);
    putText(dst_img,to_string(light_distance_val),p,FONT_HERSHEY_PLAIN,2,Scalar(255, 0, 255),1,8,false);//紫色 灯条距离
    p = Point(armor_rect.center.x,armor_rect.center.y+40);
    putText(dst_img,to_string(max_dis_th),p,FONT_HERSHEY_PLAIN,2,Scalar(255, 255, 0),1,8,false);//蓝色 最大高的n倍做上限
    p = Point(armor_rect.center.x,armor_rect.center.y+60);
    putText(dst_img,to_string(light_dis_equ_maxh),p,FONT_HERSHEY_PLAIN,2,Scalar(0, 100, 255),1,8,false);//橙色 灯条距离与最大高之比
    p = Point(armor_rect.center.x,armor_rect.center.y+80);
    putText(dst_img,to_string(w1_equ_w2),p,FONT_HERSHEY_PLAIN,2,Scalar(0, 255, 255),1,8,false);//黄色 灯条宽之比
    p = Point(armor_rect.center.x,armor_rect.center.y+100);
    putText(dst_img,to_string(armor.depth),p,FONT_HERSHEY_PLAIN,2,Scalar(194, 158, 241),1,8,false);//粉色 深度距离信息
    #endif

    if(kf_reset_cnt > 20){
        // kf.reset();
        kf_reset_cnt = 0;

    }

    float armor_area = armor.rect.size.area();
    float light_height = left_light.rect.size.height;   
    float armor_width = armor.rect.size.width;

    float distance = pinhole_test.getfitDistance(armor_width,armor_area,light_height);

    // float distance_f = 2000;
    // float true_width = 240;
    // float focal = pinhole_test.getfocalLength(armor_width,distance_f,true_width);
    // pinhole_test.getDistance(armor_width,focal,true_width);

    Point p = Point(100,100);
    putText(src_img,"current distance:   " + to_string(distance/1000),p,FONT_HERSHEY_PLAIN,2,Scalar(255, 255, 255),1,8,false);
    // p = Point(100,200);
    // putText(src_img,"armor depth distance:   " + to_string(armor.depth/1000),p,FONT_HERSHEY_PLAIN,2,Scalar(255, 255, 255),1,8,false);


    //发送串口数据
    #if IS_SERIAL_OPEN == 1
    SerialPort::RMserialWrite(_yaw_data,yaw_data, _pitch_data,pitch_data, distance, is_last_data_catch, shooting);// SerialPort::RMserialWrite(_yaw_data, abs(yaw_data), _pitch_data, abs(pitch_data), armor.depth, is_last_data_catch, shooting);
    #endif

    #if SHOW_OUTPUT_IMG == 1
    //显示图像
    imshow("src_img",src_img);
    imshow("dst_img",dst_img);
    //imshow("roi_img",roi_img);
    #endif
}

#if IS_NUMBER_PREDICT_OPEN == 1
/**
 * @brief 将Mat 转换为 Tensor
 *
 * @param src 输入的Mat
 * @param output_tensor 输出的Tensor
 * @param input_rows 输入图像的高度
 * @param input_cols 输入图像的宽度
 * @return Mat
 */
Mat RM_ArmorFitted::CVMat_to_Tensor(Mat src, Tensor* output_tensor, int input_rows, int input_cols){
    Mat img_28x28;
    resize(src,img_28x28,cv::Size(input_cols,input_rows));
    // 归一化
    img_28x28.convertTo(img_28x28, CV_32FC1);
    img_28x28 = img_28x28/255;

    float *p = output_tensor ->flat<float>().data();

    Mat tempMat(input_rows, input_cols, CV_32FC1, p);
    img_28x28.convertTo(tempMat, CV_32FC1);

    return img_28x28;
}

/**
 * @brief 输入处理好的图片并识别其中的数字
 * @param roi_img 输入的二值图像
 * @return int 预测的结果 0~9的数字
 * @note 数字识别模型需要根据实际情况重新训练数据
 *       DNN模块中有直接读取tf模型的方法，不需要很麻烦的tf配置，可以作为一个改进方向
 */
int RM_ArmorFitted::getLabel(Mat roi_img){
    Mat ID = roi_img * 5;
    Mat roi_img_gray;
    cvtColor(ID, roi_img_gray, COLOR_BGR2GRAY);
    resize(roi_img_gray,roi_img_gray,cv::Size(100,100),INTER_NEAREST);
    imshow("roi_img_gray",roi_img_gray);
    threshold(roi_img_gray,roi_img_gray,20,255,THRESH_OTSU + THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(3,3));
    dilate(roi_img_gray,roi_img_gray,element);
    element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
    erode(roi_img_gray,roi_img_gray,element);
    imshow("roi_img_bin",roi_img_gray);

    Tensor resizer_tensor(DT_FLOAT, TensorShape({1,28,28,1}));
    Mat img_28x28 = CVMat_to_Tensor(roi_img_gray, &resizer_tensor, 28, 28);

    vector<tensorflow::Tensor> outputs;
    string output_node = output_tensor_name;
    Status status_run = session -> Run({{input_tensor_name, resizer_tensor}},{output_node},{},&outputs);
    if(!status_run.ok()){
        cout << "ERROR: RUN failed..."  << endl;
        cout << status_run.ToString() << endl;
        return -1;
    }
    Tensor t = outputs[0];      // 获取第一个 tensor
    auto tmap = t.tensor<float, 2>();       //Tensor Shape:[batch_size, target_class_num]
    int output_dim = t.shape().dim_size(1);     //从第一维度获取目标类数

    //argmax: 得到最终的预测标记和概率
    int output_class_id = -1;
    double output_prob = 0.0;
    for (int j = 0; j<output_dim;++j){
        //cout << "Class " << j << " prob:" << tmap(0, j) << endl;
        if (tmap(0, j) >= output_prob) {
            output_class_id = j;
            output_prob = tmap(0, j);
        }
    }
    // 输出结果
    cout << "The Number is:[" << output_class_id <<"]"<< endl;
    return output_class_id;
}
#endif
