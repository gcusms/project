#ifndef DATA_EXCHANGE_H
#define DATA_EXCHANGE_H

#include "configure.h"
#include "control/debug_control.h"

class Data_exchange
{
private:
    /* data */
    vector<float>yvalue;
    vector<float>xvalue;

    Mat data_csv;

    float y_value_temp;
    bool record_flag = false;

public:
    Data_exchange(){};
    ~Data_exchange(){};

    void getvalueAutomatically(float &x_value,float &y_value);
    void getvalueManually(float &x_value);
    void writeinCSV();
};






#endif // !DATA_EXCHANGE_H
