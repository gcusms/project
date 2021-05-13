#pragma once

#include "Basis/RM_Control/Debug_Controller.h"
#include <iostream>
#include"opencv4/opencv2/opencv.hpp"
#include<string>




/**
 * 录制文件的参数设置
 * @param Image_cols   ---列
 * @param Image_rows ---行
 * @param USING_RECORD      ---录制开关xml
 * @param RECORD_PATH       ---路径
 * 
*/
typedef struct RM_VideoRecord
{
    int Image_cols;
    int Image_rows;
    int USING_RECORD;
    std::string RECORD_PATH;
    RM_VideoRecord()
    {
        USING_RECORD = 1;
    }
}RM_VideoRecord;



class VideoRecord
{
public:
    VideoRecord();
    ~VideoRecord(){};
    // int Width = 1200;
    // int Heigth =800;
};


