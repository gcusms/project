#ifndef SOLVE_PNP_H
#define SOLVE_PNP_H
#include "configure.h"
#include "control/debug_control.h"
#include "filter/rm_kalmanfilter.h"


class RM_SolveAngle{

public:
    RM_SolveAngle();
    ~RM_SolveAngle();
    //调用解算函数
    void run_SolvePnp(RotatedRect &rect, float _W, float _H);
    void run_SolvePnp_Buff(vector<Point2f> &image_point, Mat & srcImg,  float buff_angle, float _W, float _H);//大神符

    void draw_Coordinate(Mat & input);
    float angle_x, angle_y, dist;

     float getBuffPitch(float dist, float tvec_y, float ballet_speed);//计算子弹下坠

private:
    void vertex_Sort(RotatedRect & box);
    Mat camera_ptz(Mat & t);

    void get_Angle(const Mat & pos_in_ptz);
    void get_Angel_Buff(const Mat & pos_in_ptz, float buff_angle);//大神符

    //标定数据
    string file_path = CAMERA_PARAM_FILE;

    Mat cameraMatrix, distCoeffs;
    Mat rvec = Mat::zeros(3, 3, CV_64FC1);
    Mat tvec = Mat::zeros(3, 1, CV_64FC1);

    vector<Point3f> object_3d;
    vector<Point2f> target2d;

    const float ptz_camera_x = PTZ_CAMERA_X;
    const float ptz_camera_y = PTZ_CAMERA_Y;
    const float ptz_camera_z = PTZ_CAMERA_Z;
    const float barrel_ptz_offset_x = 0;
    const float barrel_ptz_offset_y = 0;
    const float overlap_dist = 0;

};

#endif // SOLVE_PNP_H