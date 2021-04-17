#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "control/debug_control.h"

#include "CameraApi.h"
/*---工业相机中使用到opencv2.0的 IplImage 需要包含此头文件 ---*/
#include "opencv2/imgproc/imgproc_c.h"
/*---工业相机中使用到opencv2.0的 cvReleaseImageHeader 需要包含此头文件 ---*/

/*---- OpenCV header files ----*/
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
/*---- OpenCV header files ----*/

/*---- Others header files ----*/
#include <cmath>
#include <math.h>
#include <iostream>
// #include<conio.h>
#include <stdio.h>
// #include</usr/include/eigen3/Eigen/Dense>
/*---- Others header files ----*/

/*---- Serial header files ----*/
#include <string.h>
#include <fcntl.h>  //文件控制定义
#include <termios.h>   //POSIX终端控制定义
#include <unistd.h>    //UNIX标准定义
#include <errno.h>     //ERROR数字定义
#include <sys/select.h>
/*---- Serial header files ----*/

#if IS_NUMBER_PREDICT_OPEN == 1
/*---- TensorFlow header files ----*/
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <fstream>
#include <utility>

#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"

#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"

#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"

#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"

#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"

#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
/*---- TensorFlow header files ----*/
#endif

using namespace std;
using namespace cv;
// using namespace Eigen;
#if IS_NUMBER_PREDICT_OPEN == 1
using namespace tensorflow::ops;
using namespace tensorflow;
using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
#endif

#endif // CONFIGURE_H
