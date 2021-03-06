/**
 * @file rm_videocapture.h
 * @author GCUROBOT-Visual-Group (GCUROBOT_WOLF@163.com)
 * @brief RM 2019 步兵视觉相机配置头文件
 * @version 1.1
 * @date 2019-05-06
 * @copyright Copyright (c) 2019 GCU Robot Lab. All Rights Reserved.
 */
#ifndef RM_VIDEOCAPTURE_H
#define RM_VIDEOCAPTURE_H

#include "configure.h"
#include "control/debug_control.h"

class RM_VideoCapture
{
public:
    unsigned char *g_pRgbBuffer; //处理后数据缓存区

    int                     iCameraCounts = 1;
    int                     iStatus=-1;
    tSdkCameraDevInfo       tCameraEnumList;
    int                     hCamera;
    tSdkCameraCapbility     tCapability;      //设备描述信息
    tSdkFrameHead           sFrameInfo;
    BYTE*			        pbyBuffer;
    IplImage                *iplImage = nullptr;
    int                     channel=3;
    BOOL                    AEstate=FALSE;
    tSdkImageResolution     pImageResolution;       //相机分辨率信息

    bool                    iscamera0_open = false;

public:
    RM_VideoCapture(int cameramode);
    ~RM_VideoCapture();
    bool isindustryimgInput();
    void cameraReleasebuff();
    int cameraSet();

#if CAMERA_CONFIG != 0 
public:

    #if CAMERA_CONFIG > 1
    int exposuretime = CAMERA_EXPOSURETIME;
    #endif
    
    #if CAMERA_CONFIG == 1 || CAMERA_CONFIG == 3
    int b_gain = CAMERA_BLUE_GAIN;
    int g_gain = CAMERA_GREEN_GAIN;
    int r_gain = CAMERA_RED_GAIN;
    int gamma = CAMERA_GAMMA;
    int contrast = CAMERA_CONTRAST;
    int saturation = CAMERA_SATURATION;
    #endif 

#endif
};

#endif // RM_VIDEOCAPTURE_H
