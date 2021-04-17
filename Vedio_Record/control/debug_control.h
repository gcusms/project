#ifndef DEBUG_CONTROL_H
#define DEBUG_CONTROL_H

#define CAMERA_OR_VEDIO 0
/**
 * @brief 使用相机或者视频
 * @param: 0      相机
 * @param: 1      视频
 */

/*---------------------------------------------------*/
#define PB_MODEL_PATH "./py/model/CNN_model1.pb"
/**
  @brief: tensorflow模型路径
  @param: PB_MODEL_PATH pb文件路径
*/

/*---------------------------------------------------*/

/*---------------------------------------------------*/
#if CAMERA_OR_VEDIO == 0

#define ISOPEN_INDUSTRY_CAPTURE 0
/**
  @brief: 是否使用工业相机
  @param: 0     使用工业相机
  @param: 1     使用普通USB相机
*/

#define USB_CAPTURE_DEFULT 0
/**
  @brief: 相机的默认值
  @note: 使用普通USB相机时，Opencv的VideoCapture接口的值
*/

#else

#define ISOPEN_INDUSTRY_CAPTURE 1
/**
  @brief: 是否使用工业相机
  @param: 0     使用工业相机
  @param: 1     使用普通USB相机
*/
// #define USB_CAPTURE_DEFULT "/home/jun/workplace/录像/camera_MaxBuff02.avi" //armor_2  大小装甲-红  步兵自旋-蓝  基地步兵-蓝 camera_13 camera_17
#define USB_CAPTURE_DEFULT "./camera_MaxBuff06.avi" //armor_2  大小装甲-红  步兵自旋-蓝  基地步兵-蓝 camera_13 camera_17

#endif

#define CAMERA_EXPOSURETIME 30000 //800
#define CAMERA_RESOLUTION_COLS 1280
#define CAMERA_RESOLUTION_ROWS 800
#define CAMERA_RESOLUTION_COLS_FOV ((1280 - CAMERA_RESOLUTION_COLS) * 0.5)
#define CAMERA_RESOLUTION_ROWS_FOV ((1024 - CAMERA_RESOLUTION_ROWS) * 0.5)
/**
  @brief: 设置相机的分辨率
  @param: CAMERA_EXPOSURETIME   相机曝光时间
  @param: COLS                  为图像的宽度
  @param: ROWS                  为图像的高度
  @param: FOV                   为图像对应左上角的偏移值
  @note: 这部分相机文档中是写反的　x轴　和　y轴
         偏移值计算为 *** (相机最大分辨率 - 当前设置分辨率)/2 ***
*/
/*---------------------------------------------------*/
 #define RECORD 2
/**
  @brief: 是否录制视频
  @param: 0     不录制
  @param: 1     录制
  @param: 2     相机图片
*/
/*---------------------------------------------------*/


#endif // DEBUG_CONTROL_H
