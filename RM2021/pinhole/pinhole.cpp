#include "pinhole.h"

/**
 * @brief 获取距离
 * 
 * @param pix 像素长度
 * @param focal_length 焦距 mm 
 * @param target 目标长度 mm
 * @return float 
 */
float Pinhole::getDistance(float &pix,float &focal_length,float &target){

    distance = target * focal_length / pix;
    cout<<"distance: "<<distance<<endl;
    return distance;

}

/**
 * @brief 获取焦距
 * 
 * @param pix 像素长度
 * @param distance 实际距离 mm
 * @param target 目标长度 mm
 * @return float 
 */
float Pinhole::getfocalLength(float &pix,float &distance,float &target){

    focal_length = pix * distance / target; 
    cout<<"focal_length: "<<focal_length<<endl;
    return focal_length;

}

float Pinhole::getfitDistance(float &pix_width,float &pix_area,float &pix_height){
    
    // 装甲板宽度
    distance_width = a*(pow(pix_width,b))+c;
    //装甲板面积
    distance_area = d*(pow(pix_area,e))+f;
    //灯条高度
    // distance_height = g*(pow(pix_height,h))+i;


    distance = MIN(distance_width,distance_area);
    // distance = MIN(distance,Pinhole::distance_height);
    cout<<"distance_width："<<distance_width<<endl;
    cout<<"distance_area："<<distance_area<<endl;
    // cout<<"distance_height："<<distance_height<<endl;
    cout<<"distance: "<<distance<<endl;

    return distance;

}