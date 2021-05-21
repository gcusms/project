
#include "buff_detect.h"

void BuffDetector::imageProcess(Mat & frame,int my_color){

    src_img = frame;
    GaussianBlur(src_img, gauss_img, Size(3, 3), 0);//高斯滤波
    points_2d.clear();
    vector<Point2f>(points_2d).swap(points_2d);//可预设容器大小，待检测
    cvtColor(src_img,gray_img,COLOR_BGR2GRAY);

    vector<Mat> bgr;
    split(gauss_img, bgr);                //图像通道分割(红蓝)

    switch (my_color)
    {
    case RED:{
        subtract(bgr[2], bgr[0], gauss_img);
        #if IS_PARAM_ADJUSTMENT == 1
            namedWindow("trackbar_img");
            cv::createTrackbar("GRAY_TH_RED:", "trackbar_img", &GRAY_TH_RED, 255, nullptr);
            cv::createTrackbar("COLOR_TH_RED:", "trackbar_img", &COLOR_TH_RED, 255, nullptr);        
            threshold(gauss_img, bin_img_color, COLOR_TH_RED, 255, THRESH_BINARY);
            threshold(gray_img, bin_img_gray, GRAY_TH_RED, 255, THRESH_BINARY);
            imshow("trackbar_img",trackbar_img);
        #else
            threshold(gauss_img, bin_img_color, THRESHOLD_BUFF_RED, 255, THRESH_BINARY);
            threshold(gray_img, bin_img_gray, THRESHOLD_GRAY_TH_RED, 255, THRESH_BINARY);

        #endif
    }break;
    case BLUE:{
        subtract(bgr[0], bgr[2], gauss_img);
        #if IS_PARAM_ADJUSTMENT == 1
            namedWindow("trackbar_img");
            cv::createTrackbar("GRAY_TH_BLUE:", "trackbar_img", &GRAY_TH_BLUE, 255, nullptr);
            cv::createTrackbar("COLOR_TH_BLUE:", "trackbar_img", &COLOR_TH_BLUE, 255, nullptr);        
            threshold(gauss_img, bin_img_color, COLOR_TH_BLUE, 255, THRESH_BINARY);
            threshold(gray_img, bin_img_gray, GRAY_TH_BLUE, 255, THRESH_BINARY);
            imshow("trackbar_img",trackbar_img);
        #else
            threshold(gauss_img, bin_img_color, THRESHOLD_BUFF_BLUE, 255, THRESH_BINARY);
            threshold(gray_img, bin_img_gray, THRESHOLD_GRAY_TH_BLUE, 255, THRESH_BINARY);
        #endif
    }break;
    default:
    {
        /* my_color为ALL_COLOR，则包括两种情况 */
        Mat bin_img_color_1;
        Mat bin_img_color_2;
        subtract(bgr[0], bgr[2], bin_img_color_1);// b - r
        subtract(bgr[2], bgr[0], bin_img_color_2);// r - b
        #if IS_PARAM_ADJUSTMENT == 1
            namedWindow("trackbar_img");
            cv::createTrackbar("GRAY_TH_RED:", "trackbar_img", &GRAY_TH_RED, 255, nullptr);
            cv::createTrackbar("GRAY_TH_BLUE:", "trackbar_img", &GRAY_TH_BLUE, 255, nullptr);
            cv::createTrackbar("COLOR_TH_BLUE:", "trackbar_img", &COLOR_TH_BLUE, 255, nullptr);
            cv::createTrackbar("COLOR_TH_RED:", "trackbar_img", &COLOR_TH_RED, 255, nullptr);
            int th = int((GRAY_TH_RED +GRAY_TH_BLUE)*0.5);
            threshold(gray_img, bin_img_gray, th, 255, THRESH_BINARY);
            threshold(bin_img_color_1, bin_img_color_1, COLOR_TH_BLUE, 255, THRESH_BINARY);
            threshold(bin_img_color_2, bin_img_color_2, COLOR_TH_RED, 255, THRESH_BINARY);
            imshow("trackbar_img",trackbar_img);
        #else
            int th = int((THRESHOLD_GRAY_TH_BLUE + THRESHOLD_GRAY_TH_RED)*0.5);
            threshold(gray_img, bin_img_gray, th, 255, THRESH_BINARY);
            threshold(bin_img_color_1, bin_img_color_1, THRESHOLD_BUFF_BLUE, 255, THRESH_BINARY);
            threshold(bin_img_color_2, bin_img_color_2, THRESHOLD_BUFF_RED, 255, THRESH_BINARY);
        #endif
            bitwise_or(bin_img_color_1,bin_img_color_2,bin_img_color);// 求并集
            bin_img_color_1.release();
            bin_img_color_2.release();
    }break;
    }

    #if SHOW_BIN_IMG == 1
    imshow("bin_img_color", bin_img_color);
    imshow("bin_img_gray",bin_img_gray);
    #endif 
    //    if(th-10 > 0)
    //        threshold(gaussImg, binImg, th-15, 255,  0);

    dilate(bin_img_color, bin_img_color, getStructuringElement(MORPH_RECT, Size(3,3)));    //膨胀 
    
    dilate(bin_img_gray, bin_img_gray, getStructuringElement(MORPH_RECT, Size(5,5)));    //膨胀 
    erode(bin_img_gray, bin_img_gray, getStructuringElement(MORPH_RECT, Size(3,3)));    //膨胀 

    bitwise_and(bin_img_color, bin_img_gray, bin_img_color); 
    dilate(bin_img_color, bin_img_color, getStructuringElement(MORPH_RECT, Size(3,3)));    //膨胀 晚上
    // dilate(bin_img_color, bin_img_color, getStructuringElement(MORPH_RECT, Size(5,5)));    //膨胀  早上
    erode(bin_img_color,bin_img_color,getStructuringElement(MORPH_RECT, Size(3,3)));

    bin_img_color.copyTo(bin_img);
    #if SHOW_BIN_IMG == 1
    imshow("bin_img_final", bin_img_color);
    #endif
    
    /*-----清空vector-----*/
    bgr.clear();
    vector<Mat>(bgr).swap(bgr);
    /*-----清空vector-----*/
    
}

bool BuffDetector::findTarget(Mat & frame){

    vector<Object> vec_target;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(bin_img, contours, hierarchy, 2, CHAIN_APPROX_NONE);
    for(int i = 0; i < (int)contours.size(); ++i){
        // 用于寻找小轮廓，没有父轮廓的跳过, 以及不满足6点拟合椭圆
        if(hierarchy[i][3]<0 || contours[i].size() < 6 || contours[static_cast<uint>(hierarchy[i][3])].size() < 6)
            continue;

        // 小轮廓面积条件
        double small_rect_area = contourArea(contours[i]);
        double small_rect_length = arcLength(contours[i],true);
        if(small_rect_length < 10)
            continue;
        if(small_rect_area < 200 || small_rect_area > 2000)//2000
            continue;
        // 大轮廓面积条件
        double big_rect_area = contourArea(contours[static_cast<uint>(hierarchy[i][3])]);
        double big_rect_length = arcLength(contours[static_cast<uint>(hierarchy[i][3])],true);
        if(big_rect_area < 300 || big_rect_area > 1e4)//1e4
            continue;
        if(big_rect_length < 50)
            continue;

        //Object object;

        object.small_rect_=fitEllipse(contours[i]);
        object.big_rect_ = fitEllipse(contours[static_cast<uint>(hierarchy[i][3])]);

        Point2f small_point_tmp[4];
        object.small_rect_.points(small_point_tmp);
        Point2f big_point_tmp[4];
        object.big_rect_.points(big_point_tmp);

        // for(int k = 0; k < 4; ++k){
        //     line(frame,small_point_tmp[k],small_point_tmp[(k+1)%4],Scalar(0,255,0),1);
        //     line(frame,big_point_tmp[k],big_point_tmp[(k+1)%4],Scalar(0,255,125),1);

        // }

        //组合符合条件的装甲板和叶片
        object.diff_angle =fabsf(object.big_rect_.angle-object.small_rect_.angle);//大小轮廓的角度差
        if(object.diff_angle<100 && object.diff_angle>80){                        //大小轮廓相互垂直
            float small_rect_size_ratio;
            if(object.small_rect_.size.width > object.small_rect_.size.height){   //计算长宽比
                small_rect_size_ratio = object.small_rect_.size.width/object.small_rect_.size.height;
            }else {
                small_rect_size_ratio = object.small_rect_.size.height/object.small_rect_.size.width;
            }
            //cout << big_rect_area << "  " << small_rect_area << endl;
            double area_ratio = (double)object.small_rect_.size.area()/(double)object.big_rect_.size.area();   //计算面积比例,yi
            //cout << area_ratio << endl;
            object.type_ = UNKOWN;
            //再次清洗目标找出叶片
            if(small_rect_size_ratio>1 && small_rect_size_ratio<3 && area_ratio>0.08f && area_ratio<0.25f){
                // for(int k=0;k<4;k++){
                // line(frame, small_point_tmp[k],small_point_tmp[(k+1)%4], Scalar(0, 0, 255), 1);
                // line(frame, big_point_tmp[k],big_point_tmp[(k+1)%4], Scalar(0, 0, 255), 1);
                // }

                //根据椭圆拟合得到的矩形进行逐个点显示
                // circle(frame,small_point_tmp[0],10,Scalar(0,0,255),-1,8);
                // circle(frame,small_point_tmp[1],10,Scalar(0,255,255),-1,8);
                // circle(frame,small_point_tmp[2],10,Scalar(255,0,0),-1,8);
                // circle(frame,small_point_tmp[3],10,Scalar(0,255,0),-1,8);

                object.type_= ACTION;
                //直接找出未激活状态目标（待优化）
                if(/*small_rect_area*9>big_rect_area && small_rect_area*3<big_rect_area*/0){
                    object.type_ = INACTION;
                    object.smallUpdate_Order();
                    //object.bigUpdateOrder();
                    for(int k=0;k<4;k++){
                         line(frame, small_point_tmp[k],small_point_tmp[(k+1)%4], Scalar(0, 0, 255), 3);
                         line(frame, big_point_tmp[k],big_point_tmp[(k+1)%4], Scalar(0, 0, 255), 1);
                    }
                }
                else{//未能找出未激活目标,将识别的激活目标逐个筛选出(间接法)
                    object.smallUpdate_Order();//更新装甲板的四个角的编号
                    object.knowYour_Self(bin_img);//用roi判断该装甲板是否已激活
                    if(object.type_ == INACTION){//若筛选出
                    // cout<<"小扇叶"<<endl;
                        for(int k=0;k<4;k++){
                             line(frame, small_point_tmp[k],small_point_tmp[(k+1)%4], Scalar(255,0, 0), 3);
                            line(frame, big_point_tmp[k],big_point_tmp[(k+1)%4], Scalar(0, 255,0 ), 1);
                        }
                        //object.bigUpdate_Order();
                    }
                    else {//未能筛选出未激活状态目标
                                //                        for(int k=0;k<4;k++){
                                // line(img, small_point_tmp[k],small_point_tmp[(k+1)%4], Scalar(0, 255, 255), 1);
                                // line(img, big_point_tmp[k],big_point_tmp[(k+1)%4], Scalar(0, 0, 255), 1);
                                //   }
                    }
                }
                //所有识别目标装进容器,无论是否是激活状态
                vec_target.push_back(object);    //vec_target的末尾加入object
            }
        }
    }
    //cout << vec_target.size() << endl;
    //    Object final_target;
    bool is_target = false;
    for(int i2 = 0; i2 < (int)vec_target.size(); i2++){
        object_tmp = vec_target.at(i2);
        if(object_tmp.type_ == INACTION){//清洗容器得出未激活的目标
            is_target = true;
            final_target = vec_target.at(i2);//当前目标
            buff_angle_ = final_target.angle_;//当前目标的角度
            points_2d = final_target.points_2d_;//当前目标的点
            //big_points_2d = final_target.big_points_2d_;
            target_center = final_target.small_rect_.center;//获取小轮廓的圆心
            current_point = target_center;//test angle bug
            displacement = pointDistance(current_point,last_point);
            // cout<<"是否有位移,位移为:"<<displacement<<endl;
            last_point = target_center;//test angle bug

            //circle(img, big_points_2d[1], 2, Scalar(0,255,255), 2, 8, 0);
            Point2f small_vector = points_2d[0] - points_2d[3];//获取装甲板的高度
            roi_center = final_target.big_rect_.center-BIG_LENTH_R*small_vector;//根据大轮廓的中心加上一定距离得到假定圆心,可修改
            solve_rect = final_target.small_rect_;//获取当前小轮廓的数据
            line(frame, final_target.big_rect_.center, roi_center, Scalar(0,255,255),2);//画出大轮廓到假定圆心的路径
            circle(frame, roi_center, 2, Scalar(0,0,255), 2, 8, 0);//画出假定圆心
            //circle(frame, points_2d[3], 2, Scalar(0,0,255), 2, 8, 0);

            /* -----显示内轮廓----- */
            // Point2f small_rect_point_tmp[4];
            // final_target.small_rect_.points(small_rect_point_tmp);
            // for(int k = 0; k < 4; ++k){
            // line(frame,small_rect_point_tmp[k],small_rect_point_tmp[(k+1)%4],Scalar(0,255,0),3);
            // }
            /* -----显示内轮廓----- */
        }
    }

    /*----- vector 清除内容 -----*/
    contours.clear();
    hierarchy.clear();
    vec_target.clear();

    vector<Object>(vec_target).swap(vec_target);
    vector<Vec4i>(hierarchy).swap(hierarchy);
    vector<vector<Point>>(contours).swap(contours);
    /*----- vector 清除内容 -----*/

    // imshow("src", frame);
    return is_target;
}

//待改进,暂时想到比较中心点到roi中心最小的
bool BuffDetector::findCenter_R(Mat &bin_img, Mat &frame){
    bool is_circle = false;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    RotatedRect circle_rect = RotatedRect(Point2f(frame.cols,frame.rows),Size2f(0,0),0);
    Point2f frame_center(frame.cols/2,frame.rows/2);

    float last_min_distance_target =frame.rows/2;
    int last_min_area_target = frame.size().area();
    float distance_target = 0;
    int area_target = 0;
    double rect_ratio;//比较宽/高
    Point2f circle_r[4];//圆心R的点

    vector<RotatedRect>first_screen;

    findContours(bin_img, contours, hierarchy, 0, CHAIN_APPROX_NONE);
    imshow("R_bin",bin_img);
    cout<<"轮廓数目："<<contours.size()<<endl;

    for(int j = 0; j < (int)contours.size(); ++j){
        double circle_area = contourArea(contours[j]);

        if(circle_area > 2000 || circle_area < 500)//原来为30 有bug
            continue;
        RotatedRect temp_circle_rect = fitEllipse(contours[j]);
        
        // float temp_circle_width = temp_circle_rect.size.width;
        // float temp_circle_height = temp_circle_rect.size.height;
        // float temp_circle_center_x = temp_circle_rect.center.x;
        // float temp_circle_center_y = temp_circle_rect.center.y;

        // // 类似roi的条件
        // // if(temp_circle_center_x - temp_circle_width <= 0 || temp_circle_center_y - temp_circle_height <= 0 || 
        // //     temp_circle_center_x + temp_circle_width >= bin_img.cols || temp_circle_center_y + temp_circle_height >= bin_img.rows){
        // //         cout<<"77-----------------------------------------7"<<endl;
        // //         continue;
        // //     }

        // cout<<"temp_circle_rect.boundingRect().area() = "<<temp_circle_rect.boundingRect().area() <<endl;
        // // cout<<"hierarchy["<<j<<"]="<<hierarchy[j]<<endl;
        // if((temp_circle_rect.boundingRect().area() >2500 || temp_circle_rect.boundingRect().area() <1000) && hierarchy[j][3] != -1){
        //     continue;
        // }

        // // temp_circle_rect.points(circle_r);
        // // for(int k = 0;k<4;k++)
        // // {
        // //     line(frame, circle_r[k],circle_r[(k+1)%4], Scalar(0, 255, 0), 3);
        // // }

        // if(temp_circle_rect.size.width > temp_circle_rect.size.height){
        //     temp_circle_width = temp_circle_rect.size.width;
        //     temp_circle_height = temp_circle_rect.size.height;
        // }else {
        //     temp_circle_width = temp_circle_rect.size.height;
        //     temp_circle_height = temp_circle_rect.size.width;
        // }

        // rect_ratio = temp_circle_rect.size.width/temp_circle_rect.size.height;
        // // cout<<"temp_circle_rect.size["<<j<<"]="<<temp_circle_rect.size.area()<<endl;
        // cout<<"rect_ratio["<<j<<"]="<<rect_ratio<<endl;

        // // 比例适度要修正.原来为1.1f 1.12f
        // if(rect_ratio > 0.9f && rect_ratio < 1.2f){
            first_screen.push_back(temp_circle_rect);
        // }
    }

//    cout<<"符合比例条件的:"<<first_screen.size()<<endl;
    for(std::size_t i = 0; i < first_screen.size(); ++ i )
    {
        distance_target = pointDistance(first_screen[i].center,frame_center);
        cout<<"distance = "<<distance_target<<"  ";
        // area_target = first_screen[i].size.area();
        if( distance_target < last_min_distance_target /* && area_target < last_min_area_target */ )
        {
                last_min_distance_target = distance_target;
                // last_min_area_target = area_target;
                circle_rect = first_screen[i];
        }
    }

    cout<<"last_min_distance_target = "<<last_min_distance_target<<endl;
    circle_rect.points(circle_r);

    circle_center = circle_rect.center;

    is_circle = true;//待确定该标志位的位置
    for(int k = 0;k<4;k++)
    {
        line(frame, circle_r[k],circle_r[(k+1)%4], Scalar(0, 255, 0), 3);
    }
//        drawContours(img, contours, j, Scalar(0,255,0),2);
    ellipse(frame,circle_rect,Scalar(0,255,233),2,8);

    // vector<vector<Point>>().swap(contours);
    // vector<RotatedRect>().swap(first_screen);

    contours.clear();
    hierarchy.clear();
    first_screen.clear();

    vector<RotatedRect>(first_screen).swap(first_screen);
    vector<Vec4i>(hierarchy).swap(hierarchy);
    vector<vector<Point>>(contours).swap(contours);

    return is_circle;
}

int BuffDetector::buffDetect_Task(Mat &frame,int my_color){

    buff_fps.starttheTime();
    imageProcess(frame,my_color);                     //预处理
    buff_fps.endtheTime();
        if((1/g_time<40))
        // buff_fps.displayframeRate();

    buff_fps.starttheTime();
    bool is_target = findTarget(frame);      //查找目标
    buff_fps.endtheTime();
        if((1/g_time<40))
        // buff_fps.displayframeRate();
        
    buff_fps.starttheTime();


    int common = 0;
    Mat result_img;
    Mat roi_img;
//    Mat roi_power_img;//test
    bin_img.copyTo(result_img);
    frame.copyTo(roi_img);
//    frame.copyTo(roi_power_img);//test

    #if IS_PARAM_ADJUSTMENT == 1
    Mat BuffParam = Mat::zeros(1,1200, CV_8UC1);
    namedWindow("BuffParam",WINDOW_AUTOSIZE);
    createTrackbar("fire_max_cnt","BuffParam",&auto_control.fire_task.max_cnt_,200,nullptr);
    createTrackbar("reset_cnt","BuffParam",&auto_control.reset_task.max_cnt_,200,nullptr);
    createTrackbar("repeat_time","BuffParam",&auto_control.fire_task.repeat_time,2000,nullptr);
    createTrackbar("fire","BuffParam",&auto_control.fire_task.fire_flag,1,nullptr);
    createTrackbar("repeat fire","BuffParam",&auto_control.fire_task.repeat_fire_flag,1,nullptr);
    imshow("BuffParam",BuffParam);
    BuffParam.release();
    #endif
    namedWindow("offset");
    Mat offset_img = Mat::zeros(1,1200,CV_8UC1);
    createTrackbar("offset_x/100 ","offset",&offset_x,500,nullptr);
    createTrackbar("offset_y/100 ","offset",&offset_y,500,nullptr);
    createTrackbar("正1右，负0左_x ","offset",&_offset_x,1,nullptr);
    createTrackbar("正1下，负0上_y ","offset",&_offset_y,1,nullptr);
    createTrackbar("（倍数）offset_amplitude ","offset",&offset_amplitude,50,nullptr);
    createTrackbar("（函数幅值）offset_excursion ","offset",&offset_excursion,100,nullptr);
    createTrackbar("offset_preangle ","offset",&offset_preangle,100,nullptr);
    createTrackbar("offset_ratio ","offset",&offset_ratio,50,nullptr);


    imshow("offset",offset_img);
    offset_img.release();


    if(is_target){//可找到未激活目标
        if(roi_center.x < 0 || roi_center.y < 0 || roi_center.x > frame.cols || roi_center.y > frame.rows){
            if(roi_center.x < 0){
                roi_center.x = 1;
            }else if (roi_center.y < 0){
                roi_center.y = 1;
            }else if (roi_center.x > frame.cols){
                roi_center.x = frame.cols - 1;
            }else if (roi_center.y > frame.rows){
                roi_center.y = frame.rows - 1;
            }
        }
        RotatedRect roi_R(roi_center, Size(90,90),0);//画出假定圆心的roi矩形
        Rect roi = roi_R.boundingRect();
        //roi安全条件
        if(roi.tl().x < 0 || roi.tl().y < 0|| roi.br().x > frame.cols || roi.br().y > frame.rows){
            Point TL = roi.tl();
            Point BR = roi.br();
            if(roi.tl().x < 0 || roi.tl().y < 0){
                if(roi.tl().x<0){
                    TL.x = 1;
                    // cout<<"左";
                    if(roi.tl().y<0){
                        TL.y = 1;
                        // cout<<"上";
                    }
                    if(roi.br().y>frame.rows){
                        BR.y = frame.rows-1;
                        // cout<<"下";
                    }
                    // cout<<endl;
                }
                else if(roi.tl().y<0){
                    TL.y = 1;
                    if(roi.br().x>frame.cols){
                        BR.x = frame.cols-1;
                        // cout<<"右";
                    }
                    if(roi.tl().x<0){
                        TL.x = 1;
                        // cout<<"左";
                    }
                    // cout<<"上";
                    // cout<<endl;
                }
            }
            else if(roi.br().x > frame.cols || roi.br().y > frame.rows){
                if(roi.br().x>frame.cols){
                    BR.x = frame.cols-1;
                    // cout<<"右";
                    if(roi.br().y>frame.rows){
                        BR.y = frame.rows-1;
                        // cout<<"下";
                    }
                    if(roi.tl().y<0){
                        TL.y = 1;
                        // cout<<"上";
                    }
                    // cout<<endl;
                }
                else if(roi.br().y>frame.rows){
                    BR.y = frame.rows-1;
                    if(roi.tl().x<0){
                        TL.x = 1;
                        // cout<<"左";
                    }
                    if(roi.br().x>frame.cols){
                        BR.x = frame.cols-1;
                        // cout<<"右";
                    }
                    // cout<<"下";
                    // cout<<endl;
                }
            }
            roi = Rect(TL, BR);
        }

        bin_img(roi).copyTo(result_img);
        frame(roi).copyTo(roi_img);
        rectangle(frame,roi,Scalar(0,255,200),2,8,0);      //画出roi区域(在frame)

        ++find_cnt_;
        if(find_cnt_%2 == 0){//隔帧读数据
            direction_tmp_ = getState();//判断旋转方向 1顺时针,-1逆时针
            if(find_cnt_ == 10)
                find_cnt_ = 0;
        }


        if(1)//大神符加速函数 隔帧进行处理
        {
            pre_angle_large = preangleoflargeBuff();
            pre_angle_large = red_box - offset_excursion/10;
        }

        bool is_circle = findCenter_R(result_img, roi_img);

        if(is_circle == true){
            double total;
            //小轮廓圆心和R的斜率的反正切，得到一个角
            // double theta = atan(double(target_center.y - circle_center.y) / (target_center.x - circle_center.x));

            /* --------------test--------------- */

            // cout<<"y = "<<target_center.y - (circle_center.y+roi.tl().y)<<"  x = "<<target_center.x - (circle_center.x+roi.tl().x)<<endl;
            //test_theta 装甲板中点与中心R的夹角，角度0°在正左方，顺时针增加到360°，且与0°重合
            double test_theta = atan2(target_center.y - (circle_center.y+roi.tl().y),target_center.x - (circle_center.x+roi.tl().x))*180/CV_PI + 180;
            // cout<<"test_theta = "<<test_theta<<"  buff_angle = "<<buff_angle_<<endl;

            //固定预测角的角度
            double test_total_angle =  direction_tmp_*PRE_ANGLE+test_theta;
            if(test_total_angle < 0){
                test_total_angle += 360;
            }else if (test_total_angle > 360)
            {
                test_total_angle -= 360;
            }
            // cout<<"test_total_angle = "<<test_total_angle<<endl;

            //固定预测角转为弧度
            double test_total = direction_tmp_*(test_total_angle)*CV_PI/180;
            // cout<<"test_total = "<<test_total<<endl;
            // double sin_calcu_test = sin(test_total);
            // double cos_calcu_test = cos(test_total);

            // cout<<"sin : "<<asin(sin_calcu_test)*180/CV_PI<<"  cos : "<<acos(cos_calcu_test)*180/CV_PI<<endl;
            /* --------------test--------------- */
            
            //没懂，一个很神奇的坐标系
            double theta = atan2(double(target_center.y - circle_center.y) , (target_center.x - circle_center.x));//弧度
            // cout<<"theta angle = "<<theta*180/CV_PI<<"  theta :"<<theta<<endl;//test
            // double test_total;//test
            //计算预测量,隔帧取样可能bug，尝试延长采样周期
            if(direction_tmp_ != 0){
                // cout<<"pre_theta = "<<PRE_ANGLE+theta<<endl;
                total = direction_tmp_*(offset_preangle+theta)*CV_PI/180;//转换为弧度 ！！！此处加上大神符加速函数
                test_total = /* direction_tmp_* */-1*(offset_preangle+theta+pre_angle_large*offset_amplitude)*CV_PI/180;// test 待修改 1、提前量的函数（+-） 2、提前量的增益（*） 3、提前量的
                // total_angle = /* direction_tmp_* */(PRE_ANGLE+theta+pre_angle_large);//test
                // cout<<"buff_h  = "<<test_buff_h<<" total = "<<total<<endl;//test
                // cout<<"total:"<<total<<endl;
                // cout<<"pre_angle_large:"<<pre_angle_large<<endl;

            }
            else {
                total = theta*CV_PI/180;
                test_total = theta*CV_PI/180;//test
            }


            double sin_calcu = sin(total);
            double cos_calcu = cos(total);
            // cout<<"sin_calcu = "<<cos_calcu<<endl;

            double sin_calcu_test = sin(test_total);//test
            double cos_calcu_test = cos(test_total);//test

            Point2f round_center(circle_center.x+roi.tl().x, circle_center.y+roi.tl().y);//圆心R在frame中的坐标

            pre_center.x = (target_center.x-round_center.x)*cos_calcu-(target_center.y-round_center.y)*sin_calcu+round_center.x;
            pre_center.y = (target_center.x-round_center.x)*sin_calcu+(target_center.y-round_center.y)*cos_calcu+round_center.y;

            Point2f pre_center_test;//test
            pre_center_test.x = (target_center.x-round_center.x)*cos_calcu_test-(target_center.y-round_center.y)*sin_calcu_test+round_center.x;//test
            pre_center_test.y = (target_center.x-round_center.x)*sin_calcu_test+(target_center.y-round_center.y)*cos_calcu_test+round_center.y;//test

            double radio = pointDistance(round_center, pre_center);

            //手动计算yaw角度
            float theta_pre_frame_center_x = fabs(frame.size().width/2 - pre_center_test.x) * 4.8*pow(10,-3);
            yaw_test = atan2(theta_pre_frame_center_x,8)*180/CV_PI;
            if(pre_center_test.x <= frame.size().width/2){
                yaw_test = -1*yaw_test;
            }
            cout<<"yaw_test = "<<yaw_test<<endl;


            float pre_center_angle = atan2(pre_center_test.y - round_center.y,pre_center_test.x - round_center.x)*180/CV_PI + 180;
            // cout<<"pre_center_angle = "<<pre_center_angle<<endl;
            total_angle = pre_center_angle;//最终输出的目标角度
            // float test_buff_h = 800*sin(pre_center_angle*CV_PI/180)+800;//test
            // cout<<"test_buff_h = "<<test_buff_h<<endl;
            Point2f theta_center = pre_center_test - target_center;

            points_2d.at(0) = points_2d.at(0) + theta_center;
            points_2d.at(1) = points_2d.at(1) + theta_center;
            points_2d.at(2) = points_2d.at(2) + theta_center;
            points_2d.at(3) = points_2d.at(3) + theta_center;

            // circle(frame,points_2d.at(0),6,Scalar(0,0,255),-1,8);
            // circle(frame,points_2d.at(1),6,Scalar(0,255,255),-1,8);
            // circle(frame,points_2d.at(2),6,Scalar(255,0,0),-1,8);
            // circle(frame,points_2d.at(3),6,Scalar(0,255,0),-1,8);

            circle(frame, round_center, radio, Scalar(0,255,125),2,8,0);
            circle(frame, pre_center, 3, Scalar(255,0,0),3,8,0);
            line(frame, pre_center, round_center, Scalar(0,255,255),2);
            line(frame, pre_center_test, round_center, Scalar(0,255,0),2);//test
        }
        else{
            Point2f vector_pre = points_2d[0] - points_2d[1];
            if(direction_tmp_ != 0){
                Point2f vector_center = target_center - direction_tmp_ * vector_pre * SMALL_LENTH_R;
                double theta = atan(double(vector_center.y - target_center.y) / (vector_center.x - target_center.x));
                double total = (SMALL_PRE_ANGLE+theta)*CV_PI/180;
                double sin_calcu = sin(total);
                double cos_calcu = cos(total);
                pre_center.x = (vector_center.x-target_center.x)*cos_calcu-(vector_center.y-target_center.y)*sin_calcu+target_center.x;
                pre_center.y = (vector_center.x-target_center.x)*sin_calcu+(vector_center.y-target_center.y)*cos_calcu+target_center.y;

                float pre_center_angle = atan2(target_center.y - pre_center.y,target_center.x - pre_center.x)*180/CV_PI + 180;
                total_angle = pre_center_angle;//最终输出的目标角度

                circle(frame, pre_center, 3, Scalar(0,255,0),3,8,0);
                line(frame, target_center, pre_center, Scalar(255,123,0),2);
                line(frame, target_center, vector_center, Scalar(255,123,0),2);
            }
            else {
                pre_center = target_center;
            }
        }


        solve_buff.run_SolvePnp_Buff(points_2d,frame, total_angle,BUFF_WIDTH,BUFF_HEIGHT);
        #if SERIAL_COMMUNICATION_PLAN == 0
        /* 二维＋深度 */
        yaw_data = int(pre_center.x);
        pitch_data = int(pre_center.y);
        #else
        /* Angle */
        yaw_data = solve_buff.angle_x;
        // yaw_data = yaw_test;
        pitch_data = solve_buff.angle_y;

        //test 半径补偿
        if(yaw_data < 0){
            yaw_data += offset_ratio/10;
        }else
        {
            yaw_data -= offset_ratio/10;
        }

        if(pitch_data > 0){
            pitch_data += offset_ratio/10;
        }else
        {
            pitch_data -= offset_ratio/10;
        }
        //test 半径补偿


        depth = int(solve_buff.dist);

        if(_offset_x == 0){
            yaw_data = yaw_data - offset_x/100;
        }else
        {
            yaw_data = yaw_data + offset_x/100;
        }
        
        if(_offset_y == 0){
            pitch_data = pitch_data - offset_y/100;
        }else
        {
            pitch_data = pitch_data + offset_y/100;
        }


        #endif 
        _yaw_data = (yaw_data >=0 ? 1:0);
        _pitch_data = (pitch_data >=0 ? 1:0);
    }else{

    #if SERIAL_COMMUNICATION_PLAN == 0
    /* 二维＋深度 */
    yaw_data = int(frame.cols*0.5);
    pitch_data = int(frame.rows*0.5);
    #else
    yaw_data = 0;
    pitch_data = 0;
    #endif
    _yaw_data = 0;
    _pitch_data = 0;
    }
    
    // cout <<"current common is:"<<common<<endl;
    // cout<<"depth="<<depth<<endl;
    // cout<<"yaw_data="<<yaw_data<<endl;
    // cout<<"pitch_data="<<pitch_data<<endl;

    //测试卡尔曼滤波器
    // white_box = g_time*1000;
    // yaw_data = red_box;//red
    // pitch_data = white_box;//white diff_speed_4 new_speed_5
    // _yaw_data = 0;
    // _pitch_data = 0;
    common = auto_control.run(solve_buff.angle_x,solve_buff.angle_y,is_target,diff_angle_);//坐标传出位置
    if(common == 2){
        // auto_control.current_Angle(yaw_data,pitch_data,common);
    }

    //手动计算yaw
    _yaw_data = (yaw_data >=0 ? 1:0);
    _pitch_data = (pitch_data >=0 ? 1:0);

    //发送串口数据
    #if IS_SERIAL_OPEN == 1
    #if SERIAL_COMMUNICATION_PLAN == 1
    SerialPort::RMserialWrite(_yaw_data,fabs(yaw_data)*100,_pitch_data,fabs(pitch_data)*100, depth, is_target, common);
    #else
    SerialPort::RMserialWrite(_yaw_data,yaw_data,_pitch_data,pitch_data, depth, is_target, common);
    #endif
    #endif
    
    #if SHOW_OUTPUT_IMG == 1
    //imshow("roi", result_img);
    imshow("roi_img", roi_img);
    // imshow("bin", bin_img);
    imshow("img", frame);
    #endif

    result_img.release();
    roi_img.release();

    buff_fps.endtheTime();
        if((1/g_time<40)){
            // buff_fps.displayframeRate();
            // cout<<"========"<<endl;
        }

    return 0;
}

int BuffDetector::getState(){
    diff_angle_ = buff_angle_ - last_angle;
    last_angle = buff_angle_;
    if(fabs(diff_angle_)<10 && fabs(diff_angle_)>1e-6){
        d_angle_ = (1 - REVISE) * d_angle_ + REVISE * diff_angle_;
        // cout<<"d_angle_="<<d_angle_<<endl;
    }
    //cout << "d_angle_:" << d_angle_ << endl;
    if(d_angle_ > 1.5)
        return 1;
    else if(d_angle_ < -1.5)
        return -1;
    else
        return 0;
}

void Object::smallUpdate_Order(){
    points_2d_.clear();
    Point2f points[4];
    small_rect_.points(points);
    
    Point2f point_up_center = (points[0] + points[1])/2;
    Point2f point_down_center = (points[2] + points[3])/2;
    double up_distance = pointDistance(point_up_center, big_rect_.center);
    double down_distance = pointDistance(point_down_center, big_rect_.center);
    //cout << big_rect_.angle << endl;

    if(up_distance > down_distance){ //编号顺序示意图详见北理珠readme
        angle_ = small_rect_.angle;
        points_2d_.push_back(points[0]);points_2d_.push_back(points[1]);
        points_2d_.push_back(points[2]);points_2d_.push_back(points[3]);
    }
    else {
        angle_ = small_rect_.angle+180;
        points_2d_.push_back(points[2]);points_2d_.push_back(points[3]);
        points_2d_.push_back(points[0]);points_2d_.push_back(points[1]);
    }

    cout << angle_ << endl;
}

void Object::knowYour_Self(Mat &img){
    Point2f vector_height = points_2d_.at(0) - points_2d_.at(3);

    Point left_center = points_2d_.at(3) - vector_height ;
    Point right_center = points_2d_.at(2) - vector_height ;

    int width = 10;
    int height = 5;

    Point left1 = Point(left_center.x - width, left_center.y - height);
    Point left2 = Point(left_center.x + width, left_center.y + height);

    Point right1 = Point(right_center.x - width, right_center.y - height);
    Point right2 = Point(right_center.x + width, right_center.y + height);

    Rect left_rect(left1, left2);           //画出左边小roi
    Rect right_rect(right1, right2);        //画出右边小roi

    int left_intensity = getRect_Intensity(img, left_rect);     //计算左边roi内灯光强度
    int right_intensity = getRect_Intensity(img, right_rect);   //计算右边roi内灯光强度
    // cout << left_intensity << "  " << right_intensity << endl;
    if(left_intensity <= 15 && right_intensity <= 15){
        type_ = INACTION;
    }
    else {
        type_ = ACTION;
    }
}

double pointDistance(Point2f & p1, Point2f & p2){
    double Dis=pow(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2),0.5);
    return Dis;
}

int getRect_Intensity(const Mat &frame, Rect rect){
    if(rect.width < 1 || rect.height < 1 || rect.x < 1 || rect.y < 1
            || rect.width + rect.x > frame.cols || rect.height + rect.y > frame.rows)
        return 255;
    Mat roi = frame(Range(rect.y, rect.y + rect.height), Range(rect.x, rect.x + rect.width) );
//    imshow("roi ", roi);
    int average_intensity = static_cast<int>(mean(roi).val[0]);
    roi.release();
    return average_intensity;
}

/**
 * @brief 大能量机关预测量计算
 * 
 * @return double 弧度单位的提前变化量
 */
double BuffDetector::preangleoflargeBuff(){

    double pre_angle = 0.f;

    solve_rect_center = solve_rect.center;
    diff_center = pointDistance(solve_rect_center,last_solve_rect_center);
    last_solve_rect_center = solve_rect_center;

    //修正角度在360°的突变
    diff_angle_large = buff_angle_ - last_angle_large;
    // cout<<"buff_angle_ = "<<buff_angle_<<endl;
    
    //过零处理
    if(diff_angle_large > 180)
    {
        diff_angle_large -= 360;
    }
    else if (diff_angle_large < -180)
    {
        diff_angle_large += 360;
    }

    diff_angle_large = fabs(diff_angle_large);
    // cout<<"diff_angle_large="<<diff_angle_large<<endl<<" ---------- "<<endl;
    // cout<<"buff_angle_="<<buff_angle_<<endl;
    // cout<<"last_angle="<<last_angle_large<<endl;
    // cout<<"diff_angle_large="<<diff_angle_large<<endl;

    
    //检测装甲板的切换
    _is_change_target = false;
    //角度范围可以修改
    if(fabs(diff_angle_large) > 40 ){
        _filter_flag = true;

        // cout<<"diff_angle_large = "<<diff_angle_large<<endl;
    }
    else{
        if(_filter_flag == true){
            _is_change_target = true;
            _filter_flag = false;
            change_target_cnt += 1;

        }
    }
     
    //计算时间
    timing_point_1 = getTickCount();

    delay_fitting -= 1;
    if( delay_fitting < 0 ){
        delay_fitting = 0;
    }

    // if(diff_center<=0.5f){
    //     cout<<"diff_center = "<<diff_center<<endl;

    // }

    //是否切换叶片
    if( _filter_flag == false && diff_center > 0.5f ){ 
        // cout<<"change_target_cnt = "<<change_target_cnt<<endl;

        // ++ speed_cnt;
        // if(speed_cnt%3 == 0){
            spt_t = (timing_point_1 - timing_point_2)/ getTickFrequency();//现在的单位为秒

            updateData();
        //     if(speed_cnt == 12)
        //         speed_cnt = 0;
        // }


        /* -----预测部分----- */
        if(fitting_success == true)
        {
            pre_time = last_time + spt_t;

            current_time_ = 1.884*(pre_time+2.501268136);
            while (current_time_ > CV_2PI){
                current_time_ -= CV_2PI;
            }
            pre_speed = 0.785*sin(current_time_)+1.305;
            // cout<<"pre_speed = "<<pre_speed<<endl;

            //对比当前速度和预测的速度
            error_speed = pre_speed - speed_5;

            if( fabs(error_speed) < 0.5 )//误差待测 0.5->滑动条
            {
                //三角函数增加提前量
                current_time_ =  1.884*( pre_time + 2.501268136 );
                while (current_time_ > CV_2PI){
                current_time_ -= CV_2PI;
                }
                pre_angle_large = sin(current_time_);
            }
        }
        else
        {
            // cout<<"未进入预测"<<endl;
        }
        /* -----预测部分----- */

        // cout<<"delay_fitting = "<<delay_fitting<<endl;
        // cout<<"diff_center = "<<diff_center<<endl;
        // cout<<"diff_angle_large = "<<diff_angle_large<<endl;


        //对频，标志位判断
        if( delay_fitting = 0 && diff_center < 3 ){
            if( diff_speed_1 < 0 && diff_speed_2 < 0 && diff_speed_3 > 0 && diff_speed_4 > 0 ){
                first_correct_flag += 1;
            }
            else if( diff_speed_1 < 0 && diff_speed_4 > 0 ){
                first_correct_flag += 1;
            }
            else if ( diff_speed_1 > 0 && diff_speed_2 < 0 && diff_speed_3 > 0 && diff_speed_4 < 0 )
            {
                second_correct_flag += 1;
            }
            else if ( diff_speed_1 > 0 && diff_speed_2 < 0 && diff_speed_3 > 0 && diff_speed_4 > 0 )
            {
                last_correct_flag = first_correct_flag + second_correct_flag + 1;
            }
            
            if( (first_correct_flag == 1 && second_correct_flag == 1) || last_correct_flag == 4 || first_correct_flag == 2){
                
                if( first_correct_flag == 1 && second_correct_flag == 1 )
                {
                    //第三号目标为函数的最低值
                    total_time = time_4 + time_5;
                    // cout<<"--- 3 ---"<<endl;
                }
                else if ( last_correct_flag ==4 )
                {
                    //第一号目标为函数的最低值
                    total_time = time_2 + time_3 + time_4 + time_5;
                    // cout<<"--- 1 ---"<<endl;
                }
                else
                {   
                    //第二号目标为函数的最低值
                    total_time = time_3 + time_4 + time_5;
                    // cout<<"--- 2 ---"<<endl;

                }

                first_correct_flag = 0;
                second_correct_flag = 0;
                last_correct_flag = 0;

            }

            //对频成功后将周期函数从最低点开始进行计时
            current_time_ = 1.884*(total_time+2.501268136);
            // cout<<"current_time_ = "<<current_time_<<endl;
            // cout<<"total_time = "<<total_time<<endl;
            while (current_time_ > CV_2PI){
                current_time_ -= CV_2PI;
            }
            current_speed = 0.785*sin(current_time_)+1.305;
            error_speed = current_speed - speed_5;
            // cout<<"current_speed = "<<current_speed<<endl;
            // cout<<"speed_5 = "<<speed_5<<endl;
            // cout<<"error_speed = "<<error_speed<<endl;

            if( fabs(error_speed) < 0.5 && fabs(speed_5) != 0 )//误差待测 0.5->滑动条
            {
                //对频成功
                //标志位置为 true
                fitting_success = true;
                last_speed = current_speed;
                //开始计时
                if(current_time == 0){
                    current_time += total_time;//仅第一次初始化时间池，可能会有bug
                }
                current_time = total_time;//可能会有bug
                last_time = current_time;
                delay_fitting = 40;//对频成功后延迟多少帧重新对频，待测试
            }
            else
            {
                //对频失败
                // cout<<"对频失败"<<endl;
                //标志位置为 false
                fitting_success = false;
                current_time = 0;
            }
        }
        //对频，标志位判断
    }
    else
    {
            // cout<<"切换完成"<<endl;
    }
    
    timing_point_2 = getTickCount();
    last_angle_large = buff_angle_;
    // cout<<"pre_angle = "<<pre_angle<<endl;
    // cout<<"======================"<<endl;
    return pre_angle;
}

void BuffDetector::updateData(){
    
    speed_1 = speed_2;
    speed_2 = speed_3;
    speed_3 = speed_4;
    speed_4 = speed_5;

    // cout<<"speed_1 = "<<speed_1<<"  speed_2 = "<<speed_2<<"  speed_3 = "<<speed_3<<"  speed_4 = "<<speed_4;

    time_1 = time_2;
    time_2 = time_3;
    time_3 = time_4;
    time_4 = time_5;

    time_cnt += 1;
    // white_box = diff_angle_large;
    if(spt_t > 0.46 && _is_change_target == false)
    {
        spt_t = 0.25;
        // cout<<"太大"<<endl;
    }

    // time_total += spt_t;
    // time_average = time_total/time_cnt;

    // time_5 = spt_t;
    time_5 = time_average;
 
    
 

    // cout<<"g = "<<g_time<<endl;
    // speed_5 = (diff_angle_large / g_time)*100;
    //角度 200 弧度 
    // if(speed_5 > 2){
    //     speed_5 = speed_4;
    // }

    // if(time_average != 0){
    //     speed_5 = (diff_angle_large / time_average); 
    // }else
    // {
    //     speed_5 = (diff_angle_large / 0.02); 
    // }

    // if(speed_5 - speed_4 > 100 && times_cnt > 10){
        
    //     speed_5 = speed_4 + 10;

    // }else if(speed_5 - speed_4 < 100 && times_cnt > 10){
        
    //     speed_5 = speed_4 - 10;

    // }
    // white_box = diff_angle_large;

    times_cnt += 1; 
    
    white_box = diff_angle_large;
    diff_angle_large = data_kf.data_Processing(white_box);
    diff_angle_large = data_kf.data_Processing_second(diff_angle_large);

    speed_5 = (diff_angle_large / g_time)*CV_PI/180*100;

    diff_angle_large = data_kf.data_Predict(diff_angle_large);
    red_box = diff_angle_large;

    // red_box = data_kf_2.data_Processing(white_box);

    // speed_5 = filter_speed_5;

    //记录数据
    // float cnt = times_cnt;
    // record_data.getvalueAutomatically(cnt,red_box);

    // cout<<"  red_box = "<<red_box<<endl;
    // cout<<"diff_angle_large = "<<diff_angle_large<<endl;
    // cout<<"time_5 = "<<time_5<<endl;

    diff_speed_1 = speed_2 - speed_1;
    // if(diff_speed_1 >0 ){
    //     cout<<" 凸 ";
    // }else{
    //     cout<<" 凹 ";
    // }
    diff_speed_2 = speed_3 - speed_2;
    // if(diff_speed_2 >0 ){
        // cout<<" 凸 ";
    // }else{
        // cout<<" 凹 ";
    // }
    diff_speed_3 = speed_4 - speed_3;
    // if(diff_speed_3 >0 ){
    //     cout<<" 凸 ";
    // }else{
    //     cout<<" 凹 ";
    // }
    diff_speed_4 = speed_5 - speed_4;
    // if(diff_speed_4 >0 ){
    //     cout<<" 凸 "<<endl;
    // }else{
    //     cout<<" 凹 "<<endl;
    // }



}