#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat src_img  = imread("/home/sms/tu/1.jpeg");
    resize(src_img,src_img,Size(400,400));

    imshow("src_img ",src_img);
    int x, y, z,g;
//    for(int i = 0 ; i < src_img.cols; ++i)
//    {
//        for(int j= 0 ; j< src_img.rows; ++j)
//        {
//           k = src_img.at<Vec3b>(i ,j )[2];
//        }
//    }

    //rgb颜色通道显示
    x = src_img.at<Vec3b>(src_img.cols /2 ,src_img.rows/2)[0];
    y = src_img.at<Vec3b>(src_img.cols /2 ,src_img.rows/2)[1];
    z = src_img.at<Vec3b>(src_img.cols /2 ,src_img.rows/2)[2];

    cout<<"x ="<<x<<endl;
    cout<<"y  ="<<y<<endl;
    cout<<"z  ="<<z<<endl;

    //灰度图通道显示
    cvtColor(src_img,src_img,COLOR_RGB2GRAY);
    g = src_img.at<uchar>(src_img.cols /2 ,src_img.rows/2);
    cout<<"g = " << g << endl;

    waitKey(0);
}
