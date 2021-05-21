#ifndef RM_KALMANFILTER_H
#define RM_KALMANFILTER_H

#include "configure.h"
#include "control/debug_control.h"

#define ANTI_RANGE 1.01//指数增长的底数
#define A 1.0e-6//给加速度a一个限定值(-A,A)之间
#define MNC 1e-10//测量协方差矩阵R，更大会有更慢的回归
#define DEAD_BAND 0
#define SIZE_X 960
#define SIZE_Y 480
//启用pid修正
#define PID
// pid修正参数
#define WIDTH 640
#define HEIGHT 480
#define KP 0.6
#define KI 0.02
#define KD 0.1

class RM_kalmanfilter
{
public:
    RM_kalmanfilter();
    ~RM_kalmanfilter();
    Point2f point_Predict(double g_runtime,Point2d current_point);

    //pid修正
    float point_dis(Point p1, Point p2);  // 两点距离
    Point pid_Control_predictor(Point predict, Point local);  // PID控制,返回的是一个修正后的点

private:
    Mat measurement_img;//测量矩阵
    cv::KalmanFilter kalman_img;
    Point2f last_point;
//    Point2f last_predic_point;
    double runtime=(1e-2) + 0.005666666f;
    double last_v=0;
    int n=2;

    //pid修正
    int predict_x;  // 预测位x
    int predict_y;  // 预测位y

    int last_x;  // 当前装甲板x
    int last_y;  // 当前装甲板y

    int true_x;  // 修正后的x
    int true_y;  // 修正后的y

};
/******************************************************************/
class KF_buff
{
public:
        KF_buff();
        ~KF_buff();
        Point2f Predict(/*Mat src,Point2f center_point,double r,*/double g_runtime, double current_angle,int direction_tmp, Point2f target_center, Point2f round_center);
        static inline Point calcPoint(Point2f center, double R, double angle)
        {
            return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
        }
private:
    cv::KalmanFilter kf_buff;
    double buff_last_angle;
    double buff_last_v;

    Mat buff_processNoise;
    Mat buff_measurement;
    Mat buff_state; /* (phi, delta_phi) */

    double buff_runtime=(1e-2) + 0.005666666f;
};

class KF_data
{
public:
    KF_data();
    ~KF_data();
    float data_Processing(float &newvalue);
    float data_Processing_second(float &new_value);
    float data_Predict(float &value);

private:
    cv::KalmanFilter kf_buff;

    //predict
    float filtervalue;//滤波后的值
    float predictvalue;//预测值
    float newvalue;//观测值
    float coefficient_A;//状态转换矩阵，将k-1时刻的状态变到k时刻的状态
    float U_k;//运动控制量
    float coefficient_B;//控制运动控制量
    float W_k;//过程噪声

    //predict second
    float filtervalue_2;//滤波后的值
    float predictvalue_2;//预测值
    float newvalue_2;//观测值

    //update
    float P_k;//预测误差
    float Q_k;//状态矩阵的方差
    float H_k;//状态映射量
    float kalmanGain;//卡尔曼增益
    float R_k;//观测矩阵的方差
        
    //update second
    float P_k_2;//预测误差
    float Q_k_2;//状态矩阵的方差
    float H_k_2;//状态映射量
    float kalmanGain_2;//卡尔曼增益
    float R_k_2;//观测矩阵的方差

    int tf_Q_k = 337;//746
    int tf_R_k = 1348;//9255
    int tf_coefficient_B = 1;//自己考虑要在预测值中混入多少控制量
    int tf_U_k = 0;//控制量为零，这里只是对数据进行滤波处理
    int tf_filtervalue = 20;//初始值 根据速度（° 或 rad）、位移、加速度进行设置
    int tf_W_k = 0;//过程噪声（效果未知）

    int tf_Q_k_2 = 506;//746
    int tf_R_k_2 = 1738;//9255

    Mat kalman_img = Mat::zeros(1,1200,CV_8UC1);
    Mat kalman_img_2 = Mat::zeros(1,1200,CV_8UC1);

    //输出预测值
    float pre_data = 0.f;

};



#endif // RM_KALMANFILTER_H
