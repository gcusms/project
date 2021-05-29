#include<opencv4/opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

#define LENGTH 10
#define USINGPOINTS 1
typedef struct Poins
{
    int x;
    int y;
    Poins() {}
}POINTS;


void Draw_Point( Mat& src_img);
 //定义结构体数组的个数
void input_points(Mat& src_img, POINTS points[LENGTH], char keys);

void input_points(Mat& src_img, POINTS points[LENGTH] , char keys)
{

    while(1)
    {
    int ptx,pty;
    cout<<"请输入x"<<endl;
    cin>>ptx;
    cout<<"请输入y"<<endl;
    cin>>pty;
    for(auto i = 0 ; i <  LENGTH; i++)
    {
           points[i].x = ptx;
           points[i].y = pty;
    }
    for(auto i = 0; i < LENGTH ; i++)
    {
        circle(src_img,Point(points[i].x,points[i].y),3,Scalar(0,255,0),2,8);
    }
    cout<<"请输入a退出"<<endl;
    cin>>keys;
    if(keys == 'a')
    {
       break;
    }
    }

}
int main()
{
    POINTS points[LENGTH];
    while (1)
    {


        Mat src_img = Mat(Size(640,480),CV_8UC3,Scalar(255,255,255));
        line(src_img,Point(0,src_img.rows*0.5),Point(src_img.cols,src_img.rows*0.5),Scalar(255,0,0),2,8);
        line(src_img,Point(src_img.cols,src_img.rows*0.5),Point(src_img.cols*0.95,src_img.rows*0.55),Scalar(255,0,0),2,8);
        line(src_img,Point(src_img.cols,src_img.rows*0.5),Point(src_img.cols*0.95,src_img.rows*0.45),Scalar(255,0,0),2,8);

        line(src_img,Point(src_img.cols*0.5,0),Point(src_img.cols*0.5,src_img.rows),Scalar(0,0,255),2,8);
        line(src_img,Point(src_img.cols*0.5,0),Point(src_img.cols*0.45,src_img.rows*0.1),Scalar(0,0,255),2,8);
        line(src_img,Point(src_img.cols*0.5,0),Point(src_img.cols*0.55,src_img.rows*0.1),Scalar(0,0,255),2,8);

#if USINGPOINTS == 1
        Draw_Point(src_img);
        imshow("src_img",src_img);
        waitKey(100);
#endif
#if USINGPOINTS ==0
//        char key = waitKey(27);
//        if(key == 27)
//        {
//            break;
//        }else if(key == 's')
//        {
//            cout<<"请输入按键a"<<endl;
//                input_points(src_img,points,key);
//       }

        int ptx,pty;

                for(auto i = 0 ; i <  LENGTH; i++)
                {
                    cout<<"请输入x"<<endl;
                    cin>>ptx;
                    cout<<"请输入y"<<endl;
                    cin>>pty;
                   points[i].x = ptx + src_img.rows*0.5f;
                   points[i].y = pty + src_img.cols * 0.5f;
                   circle(src_img,Point(points[i].x,points[i].y),3,Scalar(0,255,0),2,8);
                }
                for(auto i = 0; i < LENGTH-1 ; i++)
                {
                    line(src_img,Point(points[i].x,points[i].y),Point(points[i+1].x,points[i+1].y),Scalar(255,255,180),2,8);
                }
                imshow("src_img",src_img);
                 waitKey(0);

#endif


            }

}


void Draw_Point( Mat& src_img)
{
    bool direction_judge = true;
    int direction = 1;
    POINTS pt[LENGTH];
    for(auto i = 0; i< LENGTH ; ++i)
    {

        if(direction_judge == true)
         {
            direction  = 1;
        }else
        {
            direction = -1;
        }
        pt[i].x = i*30 + src_img.cols*0.1f;
        pt[i].y = rand()%300 +src_img.rows * 0.2f;
        cout<<"Point = "<<pt[i].x <<","<< pt[i].y<<endl;
        if(i%2 ==0)
        {
           direction_judge  =  !direction_judge;
        }

        circle(src_img,Point(pt[i].x,pt[i].y),3,Scalar(0,255,0),2,8);
        putText(src_img,to_string(pt[i].x)+','+to_string(pt[i].y),Point(pt[i].x,pt[i].y + 10 * direction),FONT_HERSHEY_COMPLEX_SMALL,0.6,Scalar(150,150,150),1,LINE_AA);

    }
    //画圆圈
    for(auto i = 0 ; i < LENGTH -1  ; i++)
    {

        line(src_img,Point(pt[i].x,pt[i].y),Point(pt[i+1].x,pt[i+1].y),Scalar(255,255,0),2,8);
    }

    imshow("src_img",src_img);
}
