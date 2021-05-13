#pragma once
/* --- basis header files --- */
#include <math.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
/* --- basis header files --- */

/*---工业相机中使用到opencv2.0的 IplImage 需要包含此头文件 ---*/
#include <CameraApi.h>

/*---工业相机中使用到opencv2.0的 cvReleaseImageHeader 需要包含此头文件 ---*/
#include <opencv2/imgproc/imgproc_c.h>

/* --- OpenCV header files --- */
#include <opencv4/opencv2/opencv.hpp>
/* --- OpenCV header files --- */

/* --- Serial header files --- */
#include <errno.h>  //ERROR数字定义
#include <fcntl.h>  //文件控制定义
#include <string.h>
#include <sys/select.h>
#include <termios.h>  //POSIX终端控制定义
#include <unistd.h>   //UNIX标准定义
/* --- Serial header files --- */

#include "Basis/RM_Control/Debug_Controller.h"

using namespace std;
using namespace cv;