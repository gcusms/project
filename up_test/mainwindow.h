#ifndef  MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include"qcustomplot.h"
#include <QMouseEvent>

#include "QColor"
#include <QDebug>
#include <QKeyEvent>
#include <QtGlobal>//随机数的头文件引用
#include  <QPen>


//#include <opencv4/opencv2/opencv.hpp>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    QKeyEvent *key;
    QMouseEvent *m_ev;
    void UpdatePoints();
private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    QColor color1;


    int MAX_x = 2;
    int MIN_x = 2;

    int MAX_y = 2;
    int MIN_y = 2;

    int temp_y;

    QTimer *timer1;
    QPen pen;
    int sum_t = 5;

    bool Edit_judge = false;
    bool time_judge = false;
    //定义是否使用本机时间
    bool useLocal_t=1;

    bool Go_back = false;//定义是否可以达到最佳位置

    QVector<double> x;
    QVector<double> y;

    QVector<double>x_auto;
    QVector<double>t_auto;
private slots:
    void PushDate();
    void Dafult();
    int AddPoints();
    void Timer_Event();//定时器事件
     void time_end();
     void time_start();
    void GoBackJudge();//判断是否可以回到最佳位置

    void VectorClear();//vector清除


    //设置OPencv接口函数
   // void RM_AddPoints(int t,  int y );
    void mousePressEvent(QMouseEvent *);//显示信息
protected:
    double starttime;


    void timeupdate(double  time_t);//更新时间的函数
    void KeyEvent(QKeyEvent *event);
    void RM_AddPoints(int y_input,int  grapNumber);
    void UpdateDate(  int y_input, int grapNumber,bool flag);//更新值


private:


};

#endif // MAINWINDOW_H
