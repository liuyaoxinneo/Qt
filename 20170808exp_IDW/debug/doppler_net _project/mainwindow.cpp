#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //读文件操作
//从外部数据文件读取数据

    QFile file("data6.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open the file"<<endl;
    }

    //readall方法
    unsigned char input[18755];
    while(!file.atEnd())
    {
        qDebug()<<"reading begin:";
        QByteArray line=file.readAll();
        QString str(line);
        for(int j=0;j<18755;j++)
        {
        QString str2=str.section('\n',j,j);//将原string以空格为分割分为一个个小的string
        int buf2=str2.toInt();
        input[j]=(unsigned char)buf2;
        }
    }
    qDebug()<<"reading complete!";


    //窗口设置
    setWindowTitle("PHASCAN测量结果");//窗口加标题

    //画图的初始化
    //初始化
    {
    //a扫的初始化
    ui->qCustomPlot2->addGraph();//向qcustomplot里面添加一条曲线
    ui->qCustomPlot2->graph(0)->setPen(QPen(QColor(0,0,125)));//设置a扫的颜色为深蓝
    ui->qCustomPlot2->graph(0)->setLineStyle(QCPGraph::LineStyle::lsStepCenter);//设置绘线类型
    // 设置轴，和轴上的标签是否可见
    ui->qCustomPlot2->xAxis->setVisible(true);
    ui->qCustomPlot2->xAxis2->setVisible(false);
    ui->qCustomPlot2->yAxis->setVisible(true);
    ui->qCustomPlot2->yAxis2->setVisible(false);

    ui->qCustomPlot2->xAxis->setTickLabels(true);
    ui->qCustomPlot2->xAxis2->setTickLabels(false);
    ui->qCustomPlot2->yAxis->setTickLabels(true);
    ui->qCustomPlot2->yAxis2->setTickLabels(false);

    ui->qCustomPlot2->xAxis->setRange(0,80);
    ui->qCustomPlot2->yAxis->setRange(0,100);//设置y,x的范围
    ui->qCustomPlot2->xAxis->setLabel("深度(mm)");
    ui->qCustomPlot2->yAxis->setLabel("强度(%)");
    //s扫的初始化的一部分
    ui->qCustomPlot->axisRect()->setupFullAxesBox(true);//使4条轴均可显示

    //ui初始化的一部分
    //选择显示角度
    ui->label1->setText("角度");
    ui->angle_spinBox->setRange(30,60);
    ui->angle_spinBox->setSingleStep(1);

    //选择增益
    ui->gainnumber->setRange(0.0,100.0);
    ui->gainnumber->setValue((net.gain())*0.1);//获取目前的增益设置，作为调整的初值
    ui->gainnumber->setSingleStep(10);
    ui->gainlabel->setText("增益");
    }

    //以下为画图部分

    //a扫画图，画在qCustomPlot2里
        //生成，发送数据
        QVector<double> x(605),y(605);
        QVector<double> n(605),m(605);
        int anglecount;
        anglecount=(ui->angle_spinBox->value());
        for(int i=0;i<605;i++)
        {
            x[i]=i+0.0;//+0.0使其变为double型，下面的*1.0也是同理
            y[i]=1.0*input[605*(anglecount-30)+i];
            //以下改进A扫的横纵坐标，使之与仪器一致
            n[i]=0.153*i*cos(M_PI*anglecount/180);
            m[i]=y[i]*20/51;
        }
        ui->qCustomPlot2->graph(0)->setData(n,m);//为此曲线（命名为graph0）发送数据
        ui->qCustomPlot2->replot();//重绘，适用于动态图


    //s扫画图，画在qCustomPlot里
     // set up the QCPColorMap:设置色图
        QCPColorMap *colorMap = new QCPColorMap(ui->qCustomPlot->xAxis,ui->qCustomPlot->yAxis);
        int a=400;
        int nx =350;
        int ny =350;//ny,nx的理论最佳值=a*sqrt(3)/2
        int yindex,xindex;
        double angle;

        unsigned char map2[350][350];
        unsigned char buf;
        int tag1=1,tag2=2;//用于区别强度值0的来源，即建立两套平行的坐标系，一套用于标记，一套用于存放强度值画图

        colorMap->data()->setSize(nx, ny); //使得一共有nx * ny个数据点

      //colormap初始化：对所有显示区域的点
      //绘图区域内：设为0，绘图区域外：设为150（这样为黑色）
        double x1,y1;
        for (int x1Index=0; x1Index<nx; ++x1Index)
        {
          for (int y1Index=0; y1Index<ny; ++y1Index)
          {
            colorMap->data()->cellToCoord(x1Index, y1Index, &x1, &y1);
            map2[x1Index][y1Index]=0;//map2的非扇形区域的点被初始化为0
            if(((3*ny-3*y1Index)>=(x1Index*sqrt(3)))&&((y1Index-ny)>=(-x1Index*sqrt(3)))&&(pow(x1Index,2.0)+pow(y1Index-ny,2.0)<=pow(a+3,2.0)))//a+3的原因：使弧边更圆润
              {
                colorMap->data()->setCell(x1Index, y1Index,0);
                map2[x1Index][y1Index]=tag2;//map2中扇形区域内的点被初始化为tag2
              }
            else
             colorMap->data()->setCell(x1Index, y1Index,150);
          }
        }

        // 为有对应坐标的离散点赋强度值
        for(int p=0;p<31;p++)
        {
            angle=p+30;
            for(int q=0;q<605;q++)
            {
                xindex=ceil(q*a*cos(angle*M_PI/180)/604);
                yindex=ceil(q*a*sin(angle*M_PI/180)/604);
                buf=input[p*605+q];
                colorMap->data()->setCell(yindex,ny-1-xindex,buf);

                map2[yindex][ny-1-xindex]=tag1;
            }
        }

        //对绘图区域内的没有值的点插值
        //以下为插值算法
        //简单的双线性插值应用
//        double xinsert,yinsert,a1,a2,a3,a4,val;
//        for (int xIndex=0; xIndex<nx; ++xIndex)
//        {
//          for (int yIndex=0; yIndex<ny; ++yIndex)
//          {
//            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
//            if((colorMap->data()->cell(xIndex, yIndex)==0)&&((3*ny-3*yIndex)>=(xIndex*sqrt(3)))&&((yIndex-ny)>=(-xIndex*sqrt(3)))&&(pow(xIndex,2.0)+pow(yIndex-ny,2.0)<=pow(a,2.0))&&(colorMap->data()->cell(xIndex,yIndex)==0))
//              {
//                a1=(colorMap->data()->cell(xIndex, yIndex+1));
//                a2=(colorMap->data()->cell(xIndex-1, yIndex));
//                a3=(colorMap->data()->cell(xIndex, yIndex-1));
//                a4=(colorMap->data()->cell(xIndex+1, yIndex));
//                val=0.25*(a1+a2+a3+a4);
//                colorMap->data()->setCell(xIndex, yIndex,val);
//               }
//          }
//        }

        //1不寻找的反距离加权插值，8-1
//        double xinsert,yinsert,val;
//        double a0,a1,a2,a3,a4,a5,a6,a7;
//        for (int xIndex=0; xIndex<nx; ++xIndex)
//        {
//          for (int yIndex=0; yIndex<ny; ++yIndex)
//          {
//            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
//            if((colorMap->data()->cell(xIndex, yIndex)==0)&&((3*ny-3*yIndex)>=(xIndex*sqrt(3)))&&((yIndex-ny)>=(-xIndex*sqrt(3)))&&(pow(xIndex,2.0)+pow(yIndex-ny,2.0)<=pow(a,2.0))&&(colorMap->data()->cell(xIndex,yIndex)==0))
//              {
//                a0=colorMap->data()->cell(xIndex-1,yIndex+1);
//                a1=colorMap->data()->cell(xIndex-1,yIndex);
//                a2=colorMap->data()->cell(xIndex-1,yIndex-1);
//                a3=colorMap->data()->cell(xIndex,yIndex+1);
//                a4=colorMap->data()->cell(xIndex,yIndex-1);
//                a5=colorMap->data()->cell(xIndex+1,yIndex+1);
//                a6=colorMap->data()->cell(xIndex+1,yIndex);
//                a7=colorMap->data()->cell(xIndex+1,yIndex-1);
//                val=1/6*(1*(a1+a3+a4+a6)+0.5*(a0+a2+a5+a7));
//                colorMap->data()->setCell(xIndex, yIndex,val);}
//          }
//        }

        //更好的分辨率应该换用其它的插值算法
       //1寻找的反距离加权插值
//        double xinsert,yinsert,val,bufd,di=0,dj=0;
//        int step1,step2,tag3=3,count=0;
//        double beta=1;
//        int radius=2;//搜寻的最大半径
//        for (int xIndex=0; xIndex<nx; ++xIndex)
//        {
//          for (int yIndex=0; yIndex<ny; ++yIndex)
//          {
//            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
//            if((map2[xIndex][yIndex]==tag2))
//              {

//                        qDebug()<<"插值前"<<colorMap->data()->cell(xIndex,yIndex);

//                for(step1=-radius;step1<radius+1;++step1)//
//                {
//                    for(step2=-radius;step2<radius+1;++step2)
//                    {
//                        if((map2[xIndex+step1][yIndex+step2]==tag1))
//                        {
//                            count=count+1;
//                            bufd=sqrt(qPow(step2,2)+qPow(step1,2));
//                            dj=dj+qPow(bufd,-beta);
//                            di=di+(colorMap->data()->cell(xIndex+step1, yIndex+step2))*qPow(bufd,-beta);
//                        }
//                    }
//                }
//                qDebug()<<xIndex<<yIndex;
//                qDebug()<<count;

//                count=0;
//                val=di/dj;

//                        qDebug()<<"插值后"<<val;
//                colorMap->data()->setCell(xIndex,yIndex,val);
//              }
//          }
//        }


        //2以周围（radius+1）^2-1个格子中最大的值作为插值
        double xinsert,yinsert,val,bufd,di=0,dj=0,max=0,bufmax,a1,a2,a3,a4;
        int step1,step2,tag3=3,count=0;
        double beta=1;
        int radius=1;//搜寻的最大半径
        for (int xIndex=0; xIndex<nx; ++xIndex)
        {
          for (int yIndex=0; yIndex<ny; ++yIndex)
          {
            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
            if((map2[xIndex][yIndex]==tag2))
              {

                qDebug()<<"插值前"<<colorMap->data()->cell(xIndex,yIndex);

                for(step1=-radius;step1<radius+1;++step1)
                {
                    for(step2=-radius;step2<radius+1;++step2)
                    {
                        if((map2[xIndex+step1][yIndex+step2]==tag1))
                        {
                            bufmax=colorMap->data()->cell(xIndex+step1,yIndex+step2);
                            if(bufmax>max)
                                max=bufmax;
                        }
                    }
                }
                val=max;
                qDebug()<<"插值后"<<val;
                qDebug()<<"------";
                //以下条件有降噪的效果
//                if(val-(colorMap->data()->cell(xIndex,yIndex))>10)
                    colorMap->data()->setCell(xIndex,yIndex,val);
              }
            max=0;
          }
        }

        //二次平滑，这次插值查找半不变
        for (int xIndex=0; xIndex<nx; ++xIndex)
        {
          for (int yIndex=0; yIndex<ny; ++yIndex)
          {
            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
            if((map2[xIndex][yIndex]==tag2)&&(colorMap->data()->cell(xIndex,yIndex)==0))//筛选出进过插值仍然为0的带插值点
               {
                for(step1=-radius;step1<radius+1;++step1)
                {
                    for(step2=-radius;step2<radius+1;++step2)
                    {
                        if((map2[xIndex+step1][yIndex+step2]==tag1))
                        {
                            bufmax=colorMap->data()->cell(xIndex+step1,yIndex+step2);
                            if(bufmax>max)
                                max=bufmax;
                        }
                    }
                }
                val=max;
                colorMap->data()->setCell(xIndex,yIndex,val);
               }
          }
        }

        // add a color scale:
        QCPColorScale *colorScale = new QCPColorScale(ui->qCustomPlot);
        ui->qCustomPlot->plotLayout()->addElement(0,1,colorScale);
        colorScale->setDataRange(QCPRange(0,300));
        colorMap->setColorScale(colorScale);
        colorMap->setGradient(QCPColorGradient::gpPolar);

        //以下尝试自己设置色彩梯度
       QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->qCustomPlot);
       ui->qCustomPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);


       //状态栏显示当前时间
       QTime time(QTime::currentTime());
       int hour=time.hour();
       int b=time.minute();
       int c=time.second();
       ui->statusBar->showMessage(QString("当前时间: %1:%2:%3")
                                  .arg(hour)
                                  .arg(b)
                                  .arg(c)
                                  ,0);


}


MainWindow::~MainWindow()
{
    delete ui;
}

