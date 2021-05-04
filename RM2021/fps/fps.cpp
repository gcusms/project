#include "fps.h"

Fps::Fps(){
    time = 0.0;
    fps = 0;
    time_account = 0;
    fps_account = 0;
    fps_average = 0.0;
}

Fps::~Fps(){
}

void Fps::starttheTime(){

    time_1 = double(getTickCount());
    time_account += 1;

}

void Fps::endtheTime(){

    time_2 = double(getTickCount());
    time = (time_2 - time_1) / getTickFrequency();
    fps = int(1.0/time);
    

}
void Fps::displayframeRate(){

    cout<<"FPS: "<<fps<<endl;
    // printf("%%c\:",name);

}
void Fps::computetheAverage(){

    fps_account += fps;
    fps_average = fps_account / time_account;
    cout<<"FPS_average: "<< fps_average<<endl;

}
