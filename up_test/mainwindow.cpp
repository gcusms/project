#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMouseEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QTimerEvent *time_event;
    ui->setupUi(this);
    customPlot = ui->widget;  //方便调用声明

    customPlot->setBackground(QBrush(Qt::black));//设置背景颜色

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::PushDate);//函数一定要是槽函数-----重点

    connect(ui->pushButton_back,&QPushButton::clicked,this,&MainWindow::Dafult);
    connect(ui->pushButton_close,&QPushButton::clicked,this,&MainWindow::close);    //关闭


   //输入函数
    connect(ui->pushButton_input,&QPushButton::clicked,this,&MainWindow::UpdatePoints);

    //定时器
    timer1 = new QTimer (this);
   qsrand(QTime(100,10,120).secsTo(QTime::currentTime()));
    timer1->start(1);//最后根据程序传入而定
    connect(timer1,SIGNAL(timeout()),this,SLOT(Timer_Event()));
    connect(ui->pushButton_tend,&QPushButton::clicked,this,&MainWindow::time_end);  //  计时结束
    connect(ui->pushButton_tstart,&QPushButton::clicked,this,&MainWindow::time_start);      //计时开始


    //确定是否可以回到原点---正在考虑加不加
    //connect(ui->checkBox,&QPushButton::clicked,this,&MainWindow::GoBackJudge);
    //vector槽函数清除
    connect(ui->pushButton_clear,&QPushButton::clicked,this,&MainWindow::VectorClear);
    customPlot->addGraph();//增加一条线

    //确定鼠标精度
    customPlot->setSelectionTolerance(1);
    //customPlot->setMouseTracking(true);
    connect(customPlot,SIGNAL(mousePress(QMouseEvent * )),this,SLOT(mousePressEvent(QMouseEvent *)));


    //时间参数
    QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
    dateTick->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setTicker(dateTick);


    customPlot->xAxis->setLabel("x轴");
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setTickLabelRotation(30);//旋转30°
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setBasePen(QPen(Qt::white));
    customPlot->xAxis->setTickPen(QPen(Qt::white));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white));


    customPlot->yAxis->setLabel("y轴");
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setBasePen(QPen(Qt::white));
    customPlot->yAxis->setTickPen(QPen(Qt::white));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white));

    customPlot->xAxis->setRange(MIN_x,MAX_x);
    customPlot->yAxis->setRange(MIN_y,MAX_y);


     customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//放大和移动

     pen.setWidth(5);
     customPlot->addGraph();
     pen.setStyle(Qt::PenStyle::SolidLine);
     pen.setColor(Qt::red);
     customPlot->graph(0)->setPen(this->pen);




 }

 MainWindow::~MainWindow()
 {
    delete ui;
 }

 void MainWindow::PushDate()
 {

   double Numberx = ui->LineX->text().toInt();
   double Numbery = ui->LineY->text().toInt();

   if(Numberx > MAX_x)
    {
        MAX_x = Numberx;
        Edit_judge = true;
    }
    if(Numberx < MIN_x)
    {
        MIN_x = Numberx;

    }
    if(Numbery > MAX_y)
    {
        Edit_judge = true;
        MAX_y = Numbery;
    }
    if(Numbery < MIN_y)
    {
        MIN_y = Numbery;
    }

    x.push_back(Numberx);
    y.push_back(Numbery);
    customPlot->xAxis->setRange(MIN_x,MAX_x);
    customPlot->yAxis->setRange(MIN_y,MAX_y);

    customPlot->graph(0)->setData(x,y);  //画线函数
    customPlot->replot();
    qDebug()<<"x = "<<x.at(x.size() -1);
    qDebug()<<"y = "<<y.at(y.size() -1);

  }



//回到起点
void MainWindow::Dafult()
{
    qDebug()<<"hello world";
    QString message = ui->LineX->text();
    if(message.isEmpty() && !Edit_judge && Go_back)
    {
       MAX_x = 5;
       MAX_y = 5;
    }
//    customPlot->xAxis->setRange(MIN_x,MAX_x);
//    customPlot->yAxis->setRange(MIN_y,MAX_y);
    customPlot->graph(0)->rescaleAxes(true);  //回到最佳的位置
    customPlot->replot();

}


int MainWindow::AddPoints()
{
    double y =  ui->line_autor->text().toInt();
    if(MAX_y <y )
    {
        this->MAX_y = y;
    }
    if(MIN_y > y)
    {
        this->MIN_y = y;
    }
    return y;
}

//更新点的值
void MainWindow::UpdatePoints()
{
//    double t_temp = cv::getTickCount();
//    double t  = (t_temp - starttime)/cv::getTickFrequency();

//    if(MAX_x < t)
//    {
//       this-> MAX_x = t;
//    }
//    if(MIN_x > t)
//    {
//        this->MIN_x = t;
//    }


//    y.push_back(AddPoints());
//    x.push_back(t);

//    this->customPlot->xAxis->setRange(MIN_x,MAX_x);
//     this->customPlot->yAxis->setRange(MIN_y,MAX_y);

//    this->customPlot->graph(0)->setData(x,y);
//   this->customPlot->replot();

//    qDebug()<<"x = "<<x.at(x.size() -1);
//    qDebug()<<"y = "<<y.at(y.size() -1);


}


void MainWindow::Timer_Event()
{
    if(time_judge)//如果计时开始
    {

        //更新y
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int numbery = qrand()%(1000);//产生随机数
        temp_y = numbery;//存入显示变量里面

        if( numbery > MAX_y)
        {
            MAX_y = numbery;
        }
        if(numbery < MIN_y)
        {
            MIN_y = numbery;
        }

        //更新x
//        if( !Edit_judge&&message.isEmpty() )
//        {
//            sum_t +=MAX_x;
//        }
        if(!useLocal_t)
        {
            sum_t +=1;

            if( sum_t > MAX_x)
            {
                MAX_x = sum_t;
            }
            if(sum_t < MIN_x)
            {
                MIN_x = sum_t;
            }
            x.push_back(sum_t);
        }else
        {
            static unsigned int pretime = 0;
             double current_time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;//单位秒
            if(pretime != current_time)
            {
                 this->timeupdate(current_time);
                pretime = current_time;
            }


        }


        y.push_back(numbery);
//        customPlot->xAxis->setRange(MIN_x,MAX_x);
//        customPlot->yAxis->setRange(MIN_y,MAX_y);
        customPlot->graph(0)->rescaleAxes(true);  //回到最佳的位置
        customPlot->graph(0)->setData(x,y);  //画线函数
        customPlot->replot();
        //显示数据
        ui->textBrowser->clear();
        ui->textBrowser->insertPlainText(QString::number(temp_y));

    }
}

void MainWindow::timeupdate(double time_t)
{
    x.push_back((int)time_t);
//    customPlot->xAxis->setRange(time_t-10 - customPlot->xAxis->range().size(),time_t);
    customPlot->xAxis->setRange(time_t - 10, time_t);

}

void MainWindow::time_end()
{
    if(time_judge)
    {
          time_judge = false;
          timer1->stop();
    }

}

void MainWindow::time_start()
{
     if(!time_judge)
     {
        time_judge = true;
          timer1->start();
     }

}


void MainWindow::GoBackJudge()
{
    if(ui->checkBox->isChecked())
    {
               Go_back = true;
    }else
    {
              Go_back = false;
    }

}


//槽函数清除
void MainWindow::VectorClear()
{
    x.clear();
    y.clear();
    sum_t = 5;
}



//鼠标按键获取
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"鼠标点击";
    if(event->button() != Qt::LeftButton)
    {
        return ;
    }


//    //如果在区间外面点击鼠标，则设为无效
    if(!customPlot->viewport().contains(event->pos()))
    {
        return ;
    }

    double hit_y = customPlot->yAxis->pixelToCoord(event->pos().y());
    ui->mouse_hit->clear();
    ui->mouse_hit->insertPlainText("y = " +QString::number(hit_y));

}




void MainWindow::UpdateDate( int y_input, int grapNumber,bool flag)
{
    sum_t +=1;
    if(flag)
    {
          RM_AddPoints(y_input,grapNumber);
    }else
    {
            y_input = 0;
    }

    y.push_back(y_input);
    customPlot->xAxis->setRange(MIN_x,MAX_x);
    customPlot->yAxis->setRange(MIN_y,MAX_y);

    customPlot->graph(0)->setData(x,y);  //画线函数
    customPlot->replot();



}
void MainWindow::RM_AddPoints(int y_input, int grapNumber)
{

    if( y_input > MAX_y)
    {
        MAX_y = y_input;
    }
    if(y_input < MIN_y)
    {
        MIN_y = y_input;
    }

}















