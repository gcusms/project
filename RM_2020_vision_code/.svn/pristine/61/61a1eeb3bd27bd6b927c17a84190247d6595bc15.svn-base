#include "data_exchange.h"

/**
 * @brief 自动获取自变量和应变量
 * 
 * @param x_value 自变量
 * @param y_value 应变量
 */
void Data_exchange::getvalueAutomatically(float &x_value,float &y_value){

    //按s开始
    if(waitKey(1) == 115){
        record_flag = true;
        for(auto i = 0;i<5;++i){cout<<"---- 记录开始 ----"<<endl;}

    }

    //按e结束
    if(waitKey(1) == 101){
        record_flag = false;
        for(auto i = 0;i<5;++i){cout<<"---- 记录结束 ----"<<endl;}
    }

    if(record_flag == true){

        xvalue.push_back(x_value);
        yvalue.push_back(y_value);

    }


    
}

/**
 * @brief 手动获取应变量，自变量自动获取
 * 
 * @param x_value 自变量
 */
void Data_exchange::getvalueManually(float &x_value){

    //按键g
    if(waitKey(1) == 103){
        xvalue.push_back(x_value);

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        cout<<"请输入y值：";
        cin>>y_value_temp;

        yvalue.push_back(y_value_temp);

        for(auto i = 0;i<5;++i){cout<<"---------写入成功---------"<<endl;} 
    }
}

/**
 * @brief 写入CSV文件
 * 
 */
void Data_exchange::writeinCSV(){
    //按键w
    if(waitKey(1) == 119){
        data_csv = Mat(xvalue.size(),2,CV_32F);

        auto iter_xvalue = xvalue.begin();
        auto iter_yvalue = yvalue.begin();

        for(int i = 0; i < data_csv.rows; ++i){

            float* data = data_csv.ptr<float>(i);

            for(int j = 0; j < data_csv.cols; ++j){

                data[0] = *(iter_xvalue + i);
                data[1] = *(iter_yvalue + i);

            }

        }

        ofstream file ("fitting_data.csv");
        file<<format(data_csv,Formatter::FMT_CSV);
        file.close();

        for(auto i = 0;i<data_csv.rows;++i){

            float* data = data_csv.ptr<float>(i);

            for(auto j = 0;j<data_csv.cols;++j){

                cout<<"data_csv["<<i<<"]["<<j<<"]="<<(float)data[j];

            }
        cout<<endl;
        }
    }
}