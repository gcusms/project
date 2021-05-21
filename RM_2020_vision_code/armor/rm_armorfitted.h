#ifndef RM_ARMORFITTED_H
#define RM_ARMORFITTED_H

#include "configure.h"
#include "control/debug_control.h"
#include "solve_PNP/solve_pnp.h"
#include "serial/serialport.h"
#include "filter/rm_kalmanfilter.h"
#include "pinhole/pinhole.h"
#include "data_exchange/data_exchange.h"

// //结构体实现方法
// struct ArmorROI
// {
//     RotatedRect last_armor_rect = RotatedRect();      // 上一次检测到装甲的旋转矩形
//     Point tl = Point(0,0);                         // 截图区域的左上角的点
//     int width = 0;                        // 截图区域的宽度
//     int height = 0;                       // 截图区域的高度
//     Mat ROI_img;                      // ROI区域
//     /**
//      * @brief 将图像设置为上一帧的ROI区域
//      *
//      * @param src 相机捕获的原始图像
//      * @param is_Lost_target 是否丢失目标标志位
//      * @param cnt 丢失目标帧计数--以此为准扩大ROI搜索范围
//      * @param last_roi_rect 上一帧所处理处的ROI区域
//      * @param dis 深度信息--用来调整ROI区域的缩放倍率
//      */
//     void updateROI(Mat &src,bool is_Lost_target,short int cnt,RotatedRect last_roi_rect, double dis){
//         Point _tl;
//         int W,H;
//         if(!is_Lost_target){
//             if(cnt <= LOSE_CNT_MAX*0.5){
//                 W = last_roi_rect.boundingRect().width * 2;
//                 H = int(last_roi_rect.boundingRect().height * 2.5);
//             } else if (LOSE_CNT_MAX*0.5 < cnt && cnt <= LOSE_CNT_MAX){
//                 W = last_roi_rect.boundingRect().width * 3;
//                 H = int(last_roi_rect.boundingRect().height * 3.5);
//             } else{
//                 W = last_roi_rect.boundingRect().width;
//                 H = last_roi_rect.boundingRect().height;
//             }
//             /*--- ROI区域安全处理 ---*/
//             _tl = Point2f(last_roi_rect.center.x - W*0.5f,last_roi_rect.center.y - H*0.5f);
//             if (_tl.x < 0){
//                 _tl.x = 0;//如果左上角点x超出尺寸，x =  0
//             }
//             if (_tl.y < 0){
//                 _tl.y = 0;//如果左上角点y超出尺寸，y = 0
//             }
//             if (last_roi_rect.center.x - W*0.5f < 0.f){
//                 W = W + int((last_roi_rect.center.x - W*0.5f) );
//                 // 如果宽度超出了左边界，设置为到边界的值
//             }
//             if (_tl.x + W > src.cols){
//                 W = W - (_tl.x + W - src.cols);
//                 // 如果宽度超出了右边界，设置为到边界的值
//             }
//             if (last_roi_rect.center.y - H*0.5f < 0.f){
//                 H = H + int((last_roi_rect.center.y - H*0.5f) );
//                 // 如果高度超出了上边界，设置为到边界的值
//             }
//             if (_tl.y + H > src.rows){
//                 H = H - (_tl.y + H - src.rows );
//                 // 如果高度超出了下边界，设置为到边界的值
//             }
//             /*--- ROI区域安全处理 ---*/
//             Rect roi = Rect(_tl.x,_tl.y,W,H);
//             //更新成员
//             src(roi).copyTo(ROI_img);
//             last_armor_rect = last_roi_rect;
//             tl = Point2f(_tl);
//             width = W;
//             height = H;
//             //更新成员
//             rectangle(src, roi, Scalar(255,255,255),2,8,0);
//         }else{
//             //重置参数
//             src.copyTo(ROI_img);
//             last_armor_rect = RotatedRect();
//             tl = Point2f(0.f,0.f);
//             width = 0;
//             height = 0;
//         }
//     }
// };

// 类实现方法
class ArmorROI
{
public:
    RotatedRect last_armor_rect = RotatedRect();      // 上一次检测到装甲的旋转矩形
    Point2f tl = Point2f(0.f,0.f);                         // 截图区域的左上角的点
    int width = 0;                        // 截图区域的宽度
    int height = 0;                       // 截图区域的高度
    Mat ROI_img;                      // ROI区域
    #if IS_NUMBER_PREDICT_OPEN == 1
    Mat ROI_armor_ID;                 // 装甲板ID ROI区域
    #endif
    
    ArmorROI(){}

    /**
     * @brief 将图像设置为上一帧的ROI区域
     *
     * @param src 相机捕获的原始图像
     * @param is_Lost_target 是否丢失目标标志位
     * @param cnt 丢失目标帧计数--以此为准扩大ROI搜索范围
     * @param last_roi_rect 上一帧所处理处的ROI区域
     * @param dis 深度信息--用来调整ROI区域的缩放倍率 TODO:
     */
    void updateROI(Mat &src,bool is_Lost_target,short int cnt,RotatedRect last_roi_rect, double dis){
        
        Point _tl;
        int W,H;
        if(!is_Lost_target){
            if(cnt <= LOSE_CNT_MAX*0.5){
                W = last_roi_rect.boundingRect().width * 4;
                H = int(last_roi_rect.boundingRect().height * 5);//2.5
            } else if (LOSE_CNT_MAX*0.5 < cnt && cnt <= LOSE_CNT_MAX){
                W = last_roi_rect.boundingRect().width * 6;
                H = int(last_roi_rect.boundingRect().height * 7);//3.5
            } else{
                W = last_roi_rect.boundingRect().width;
                H = last_roi_rect.boundingRect().height;
            }
            /*--- ROI区域安全处理 ---*/
            _tl = Point2f(last_roi_rect.center.x - W*0.5f,last_roi_rect.center.y - H*0.5f);
            if (_tl.x < 0){
                _tl.x = 0;//如果左上角点x超出尺寸，x =  0
            }
            if (_tl.y < 0){
                _tl.y = 0;//如果左上角点y超出尺寸，y = 0
            }
            if (last_roi_rect.center.x - W*0.5f < 0.f){
                W = W + int((last_roi_rect.center.x - W*0.5f) );
                // 如果宽度超出了左边界，设置为到边界的值
            }
            if (_tl.x + W > src.cols){
                W = W - (_tl.x + W - src.cols);
                // 如果宽度超出了右边界，设置为到边界的值
            }
            if (last_roi_rect.center.y - H*0.5f < 0.f){
                H = H + int((last_roi_rect.center.y - H*0.5f) );
                // 如果高度超出了上边界，设置为到边界的值
            }
            if (_tl.y + H > src.rows){
                H = H - (_tl.y + H - src.rows );
                // 如果高度超出了下边界，设置为到边界的值
            }
            /*--- ROI区域安全处理 ---*/
            Rect roi = Rect(_tl.x,_tl.y,W,H);

            //更新成员
            src(roi).copyTo(ROI_img);
            last_armor_rect = last_roi_rect;
            tl = Point2f(_tl);
            width = W;
            height = H;
            #if IS_NUMBER_PREDICT_OPEN == 1
            #endif
            //更新成员
            rectangle(src, roi, Scalar(255,255,255),2,8,0);
        }else{
            //重置参数
            src.copyTo(ROI_img);
            last_armor_rect = RotatedRect();
            tl = Point2f(0.f,0.f);
            width = 0;
            height = 0;
            #if IS_NUMBER_PREDICT_OPEN == 1
            src.copyTo(ROI_armor_ID);
            #endif
        }
    }

    void setNumROI(Mat &src, RotatedRect &_rect){
        Mat roi_img;
        int roi_w = int(MAX(_rect.size.width,_rect.size.height)*0.75f);
        int roi_h = int(MIN(_rect.size.width,_rect.size.height)*2.0f);
        RotatedRect rect = RotatedRect(_rect.center,cv::Size(roi_w,roi_h),_rect.angle);
        Point2f verices[4];
        rect.points(verices);
        for (int j = 0; j < 4; j++){
            line(src, verices[j], verices[(j + 1) % 4], Scalar(150, 50, 100),2,8,0);
        }
        Point2f verdst[4];
        verdst[0] = Point2f(0, roi_h);
        verdst[1] = Point2f(0, 0);
        verdst[2] = Point2f(roi_w, 0);
        verdst[3] = Point2f(roi_w, roi_h);
        roi_img = Mat(roi_h, roi_w, CV_8UC1);
        Mat warpMatrix = getPerspectiveTransform(verices, verdst);
        warpPerspective(src, roi_img, warpMatrix, roi_img.size(), INTER_LINEAR, BORDER_CONSTANT);
        #if IS_NUMBER_PREDICT_OPEN == 1
        roi_img.copyTo(ROI_armor_ID);
        #endif
    };
};

// //结构体实现
// struct NiceLight
// {
//     RotatedRect rect= RotatedRect();
//     float WH_ratio = 0; //灯条的宽与高之比
//     float Area = 0;     //灯条最小包围矩形的面积
//     float depth = 0;     //到相机的距离

//     void inputParam(RotatedRect &_rect, RM_SolveAngle &angle_solve){
//         rect = _rect;
//         WH_ratio = MIN(rect.size.width,rect.size.height)/MAX(rect.size.width,rect.size.height);
//         Area = _rect.size.width * _rect.size.height;
//         angle_solve.run_SolvePnp(rect, LIGHT_SIZE_W, LIGHT_SIZE_H);
//         depth = angle_solve.dist;
//     }

//     void Reset(){
//         rect = RotatedRect();
//         WH_ratio = 0;
//         Area = 0;
//         depth = 0;
//     }
// };

//类的实现方法
class NiceLight{
public:
    NiceLight(){}
    void inputParam(RotatedRect &_rect, RM_SolveAngle &angle_solve){
        this-> rect = _rect;
        this-> Area = _rect.size.width * _rect.size.height;
        angle_solve.run_SolvePnp(rect, LIGHT_SIZE_W, LIGHT_SIZE_H);
        this-> depth = angle_solve.dist;
    }
    RotatedRect rect = RotatedRect();
    float WH_ratio = 0; //灯条的宽与高之比
    float Area = 0;     //灯条最小包围矩形的面积
    float depth = 0;     //到相机的距离
};

// //结构体实现
// struct CandidateArmor{
//     RotatedRect rect = RotatedRect();     //装甲板拟合出矩形
//     float armor_width = 0;    //装甲板宽度 --实际为两灯条中心的距离
//     float dist_to_center = 0;     //装甲板中心到图像中心的距离
//     int armor_kind = SMALL_ARMOR;     //装甲板种类 --大装甲或小装甲
//     Point convert_center = IMG_CENTER;

//     int yaw_data = 0;   //云台偏航
//     int _yaw_data = 1;  //偏航值的正负
//     int pitch_data = 0; //云台俯仰
//     int _pitch_data = 1;    //俯仰值的正负
//     int depth = 0;    //深度 -- 装甲板距离相机的距离
//     void inputParam(NiceLight &light_left, NiceLight &light_right, RM_SolveAngle &angle_solve){
//         rect = fit_Rrect(light_left.rect, light_right.rect);
//         armor_width = centerDistance(light_left.rect.center, light_right.rect.center);
//         dist_to_center = centerDistance((light_left.rect.center + light_right.rect.center)*0.5,
//                                         IMG_CENTER);
//         float _w = float(sqrt(pow(fabs(light_left.depth - light_right.depth), 2) + pow(armor_width, 2)));
//         //RotatedRect _rect = RotatedRect(rect.center,Size(_w,rect.size.height),rect.angle);
//         float temp_w = rect.size.width;
//         rect.size.width = _w;
//         angle_solve.run_SolvePnp(rect, SMALL_ARMOR_SIZE_W, SMALL_ARMOR_SIZE_H);
//         rect.size.width = temp_w;
//         depth = int(angle_solve.dist);
//         #if SERIAL_COMMUNICATION_PLAN == 0
//         /* 二维＋深度 */
//         yaw_data = int(armor_rect.center.x);
//         pitch_data = int(armor_rect.center.y);
//         #else
//         /* Angle */
//         yaw_data = int(angle_solve.angle_x);
//         pitch_data = int(angle_solve.angle_y);
//         #endif
//         _yaw_data = (yaw_data >=0 ? 0:1);
//         _pitch_data = (pitch_data >=0 ? 0:1);
//     }
//     RotatedRect fit_Rrect(RotatedRect &rect_left,RotatedRect &rect_right);
//     /**
//      * @brief 两点之间的距离
//      * @return float
//      * @note 这部分应该可优化
//      */
//     float centerDistance(Point p1, Point p2) {
//         float D = float(sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
//         return D;
//     }
// };
//类实现方法
class CandidateArmor{
public:
    CandidateArmor(){}
    void inputParam(NiceLight &light_left, NiceLight &light_right, RM_SolveAngle &angle_solve, ArmorROI &roi);
    RotatedRect fit_Rrect(RotatedRect &rect_left,RotatedRect &rect_right);

    /**
     * @brief 两点之间的距离
     * @return float
     * @note 这部分应该可优化
     */
    float centerDistance(Point p1, Point p2) {
        float D = float(sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
        return D;
    }

    RotatedRect rect = RotatedRect();     //装甲板拟合出矩形
    float armor_width = 0;    //装甲板宽度 --实际为两灯条中心的距离
    float dist_to_center = 0;     //装甲板中心到图像中心的距离
    int armor_kind = SMALL_ARMOR;     //装甲板种类 --大装甲或小装甲
    Point convert_center = IMG_CENTER;
    int depth;  //深度 -- 装甲板距离相机的距离
};

class RM_ArmorFitted:public ArmorROI, public NiceLight, public CandidateArmor
{
public:


    RM_ArmorFitted();
    ~RM_ArmorFitted();

    void imageProcessing(Mat frame, int my_color);
    void armorFitted();

public:
    float yaw_data = 0; //云台偏航
    int _yaw_data = 0; //偏航值的正负
    float pitch_data = 0; //云台俯仰
    int _pitch_data = 0; //俯仰值的正负
    int armor_depth = 0; //深度 -- 装甲板距离相机的距离

    double _t = 0.005; //上一帧运行时间

private:
    RM_SolveAngle angle_solve;
    RM_kalmanfilter kf;
    ArmorROI roi;
    CandidateArmor armor = CandidateArmor();
    NiceLight light,left_light,right_light;

    Pinhole pinhole_test;
    Data_exchange data_exchange;

    Mat src_img;
    Mat gray_img;
    Mat hsv_img;
    Mat bin_img_gray;
    Mat bin_img_color;
    Mat dst_img;
    Mat roi_img;

    //储存上一帧检测到的装甲的旋转矩形
    RotatedRect last_armor = RotatedRect();
    //上一帧是否有数据
    bool is_last_data_catch = false;
    //是否丢失目标
    bool is_Lost_target = true;
    //丢失目标时的帧计数
    short int lose_target_cnt = 0;
    short int kf_reset_cnt = 0;

    #if IS_PARAM_ADJUSTMENT == 1
    int GRAY_TH_BLUE = 80;   //蓝色装甲的阈值
    int COLOR_TH_BLUE = 100;//135
    int GRAY_TH_RED = 40;    //红色装甲的阈值
    int COLOR_TH_RED = 100;
    #endif

    float armor_aspect_ratio_th = 0.24444444444f;   //区分大小装甲的宽高比阈值

    #if IS_NUMBER_PREDICT_OPEN == 1
private:
    Mat CVMat_to_Tensor(Mat src, Tensor* output_tensor, int input_rows, int input_cols);
    int getLabel(Mat roi_img);

    bool read_model_success = true;

    string input_tensor_name="conv2d_1_input";
    string output_tensor_name="dense_2/Softmax";

    /*------------创建session------------*/
    GraphDef graphdef;
    SessionOptions opts;
    Session* session;
    Status status;//创建新会话Session

    /*------------读取模型------------*/
    Status status_load;
    Status status_create;
    #endif
};

#endif // RM_ARMORFITTED_H
