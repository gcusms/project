#include "rm_link.h"

/**
 * @brief Construct a new rm vision init::rm vision init object
 *
 */
RM_Vision_Init::RM_Vision_Init():capture(USB_CAPTURE_DEFULT),cap(ISOPEN_INDUSTRY_CAPTURE){
#if RECORD == 1
    String out_path = "./camera_MaxBuff15.avi";//目标路径
    Size size(1280,800);//要求与摄像头参数一致
    // int fourcc = writer.fourcc('X', 'V', 'I', 'D');   // 设置avi文件对应的编码格式 66 67
    int fourcc = writer.fourcc('M', 'J', 'P', 'G'); // 33 30 48
    writer.open(out_path,  fourcc, 66, size, true);//CAP_DSHOW = true
    if(writer.isOpened()){
        cout<<"正在录制"<<endl;
    }
    else{
        cout<<"录制失败"<<endl;
    }
#endif
}

/**
 * @brief Destroy the rm vision init::rm vision init object
 *
 */
RM_Vision_Init::~RM_Vision_Init(){
#if RECORD == 1
        writer.release();
#endif
    capture.release();
}

/**
 * @brief 视觉功能执行函数
 *
 */
void RM_Vision_Init::Run(){

    //get Image
    if(cap.isindustryimgInput()){
        src_img = cvarrToMat(cap.iplImage,true);//这里只是进行指针转换，将IplImage转换成Mat类型
    }else{
        capture >> src_img;
    }

    imshow("src_img",src_img);

#if RECORD == 1
    
    writer << src_img;

#endif

    cap.cameraReleasebuff();
}

#if ANALYZE_EACH_FRAME == 1
/**
 * @brief 程序继续条件
 * 
 * @return true 继续执行
 * @return false 暂停执行
 */
bool RM_Vision_Init::is_continue()
{
    bool go_on=false;
    int key =waitKey(0);
    if((char)key== 32)
    {
        go_on=true;
    }
    else
    {
        go_on=false;
    }
    return go_on;
}
#endif

/**
 * @brief 程序退出条件
 *
 * @return true 结束程序
 * @return false 继续执行
 */
bool RM_Vision_Init::is_exit()
{
    int key = waitKey(1);//8

        #if RECORD == 1
        if(char(key) == 101)
        {
            writer.release();
            cout<<"录制结束"<<endl;
        }
        #endif

    if(char(key) == 27)
        return true;
    else
        return false;
}
