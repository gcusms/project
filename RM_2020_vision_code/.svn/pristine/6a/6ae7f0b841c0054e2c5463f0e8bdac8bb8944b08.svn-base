#ifndef PINHOLE_H
#define PINHOLE_H

#include "configure.h"
#include "control/debug_control.h"

class Pinhole
{
private:


public:
    Pinhole(){};
    ~Pinhole(){};

    float focal_length = 0;
    float distance = 0;
    float distance_width = 0;
    float distance_area = 0;
    float distance_height = 0;

    /*----------------------小装甲--------------------------*/
    // 装甲板宽度
    float a = 3.19*pow(10,5);
    float b = -1.052;
    float c = 19.02;

    //装甲板面积
    float d = 2.329*pow(10,5);
    float e = -0.5446;
    float f = 37.62;
    /*----------------------小装甲--------------------------*/

    /*----------------------大装甲--------------------------*/
    // //装甲板宽度    
    // float a = 4.538*pow(10,5);
    // float b = -1.014;
    // float c = -32.86;
    //  //装甲板面积
    // float d = 1.631*pow(10,5);
    // float e = -0.4627;
    // float f = -179.4;
    /*----------------------大装甲--------------------------*/

    //灯条高度
    float g = 1.35*pow(10,5);
    float h = -1.068;
    float i = 0.924;
    

    float getDistance(float &pix,float &focal_length,float &target);
    float getfocalLength(float &pix,float &distance,float &target);
    float getfitDistance(float &pix_width,float &pix_area,float &pix_height);

};



#endif // !PINHOLE_H
