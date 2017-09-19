#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

int a=1000;
int nx =870;
int ny =870;//ny,nx的理论最佳值=a*sqrt(3)/2
unsigned char map2[870][870];
unsigned char input[18755];//全局变量
int tag0=1,tag1=2,tag2=3,tag3=4;
//tag0：扇形区域内的点
//tag1：扇形区域内初始强度值对应的点
//tag2：扇形区域内插值得到的点
//tag3：表示插值得到的点再经过一次平滑处理
int radius=6;//IDW寻找方格的半径

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //读文件操作
//从外部数据文件读取数据

    QFile file("test1.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"can't open the file"<<endl;
    }


    //readLine方法
    QString str2;
    while(!file.atEnd())
    {
        qDebug()<<"reading begin:";
        for(int j=0;j<18755;j++)
        {
            QByteArray line=file.readLine();
            QString str(line);
            str2=str.left((str.length()-1));
            int buf2=str2.toInt();
            input[j]=(unsigned char)buf2;
        }
    }
    qDebug()<<"data imported!";

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
        int yindex,xindex;
        double angle;

        colorMap->data()->setSize(nx, ny); //使得一共有nx * ny个数据点

      //colormap初始化：对所有显示区域的点
      //绘图区域内：设为0，绘图区域外：设为300（这样为黑色）
        double x1,y1;
        for (int x1Index=0; x1Index<nx; ++x1Index)
        {
          for (int y1Index=0; y1Index<ny; ++y1Index)
          {
            colorMap->data()->cellToCoord(x1Index, y1Index, &x1, &y1);
            if(((3*ny-3*y1Index)>=(x1Index*sqrt(3)))&&((y1Index-ny)>=(-x1Index*sqrt(3)))&&(pow(x1Index,2.0)+pow(y1Index-ny,2.0)<=pow(a+3,2.0)))//a+3的原因：使弧边更圆润
              {
                colorMap->data()->setCell(x1Index, y1Index,0);
                map2[x1Index][y1Index]=tag0;
              }
            else
             {
                colorMap->data()->setCell(x1Index, y1Index,300);
                map2[x1Index][y1Index]=0;
            }
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
                colorMap->data()->setCell(yindex,ny-1-xindex,input[p*605+q]);
                map2[yindex][ny-1-xindex]=tag1;
            }
        }

        //对绘图区域内的没有值的点插值
        qDebug()<<"start insert:";
        //0毕设插值方法：取周围上下左右四个点的平均值，20170423
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
//                colorMap->data()->setCell(xIndex, yIndex,val);}
//          }
//        }


        //1以周围（radius+1）^2-1个格子中最大的值作为插值，20170809
//        double xinsert,yinsert,val,max=0,bufmax;
//        int step1,step2;
//        int radius=1;//搜寻的最大半径
//        for (int xIndex=0; xIndex<nx; ++xIndex)
//        {
//          for (int yIndex=0; yIndex<ny; ++yIndex)
//          {
//            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);
//            if((map2[xIndex][yIndex]==tag2))
//              {
//                for(step1=-radius;step1<radius+1;++step1)
//                {
//                    for(step2=-radius;step2<radius+1;++step2)
//                    {
//                        if((map2[xIndex+step1][yIndex+step2]==tag1))
//                        {
//                            bufmax=colorMap->data()->cell(xIndex+step1,yIndex+step2);
//                            if(bufmax>max)
//                                max=bufmax;
//                        }
//                    }
//                }
//                val=max;

//                //以下条件有降噪的效果
//                if(val-(colorMap->data()->cell(xIndex,yIndex))>10)
//                   { colorMap->data()->setCell(xIndex,yIndex,val);}
//                map2[xIndex][yIndex]=tag3;//tag3表示是插值得到
//              }
//            max=0;
//          }
//        }

        //IDW，20170907
        double xinsert,yinsert,val=0;
        int count=1,step1,step2;
        double beta=2;//理论上可取0.5~3
        double di,d0=0,vi,wi,w0=0;
        for (int xIndex=0; xIndex<nx; xIndex++)
        {
          for (int yIndex=0; yIndex<ny; yIndex++)
          {
            colorMap->data()->cellToCoord(xIndex, yIndex, &xinsert, &yinsert);

            if((map2[xIndex][yIndex]==tag0))//若是扇形区域内的点但是还没有插值的话
            {
              for(int n=1;n<radius+1;n++)
             {
                for(step1=-n;step1<n+1;step1++)
                {
                    for(step2=-n;step2<n+1;step2++)
                    {
                        if((map2[xIndex+step1][yIndex+step2]==tag1))
                        {
                            di=1/pow((pow(step1,2)+pow(step2,2)),0.5*beta);
                            d0=d0+di;
                            vi=colorMap->data()->cell(xIndex+step1,yIndex+step2);
                            wi=vi*di;
                            w0=w0+wi;
                            count++;
                        }
                    }
                }
              }

             for(int i=0;i<count;++i)
             {
                 val=w0/d0;
             }
             colorMap->data()->setCell(xIndex,yIndex,val);

             count=1;
             val=0;
             d0=0;
             w0=0;
            }


          }
        }
        qDebug()<<"insert completed!";

        // add a color scale:
        QCPColorScale *colorScale = new QCPColorScale(ui->qCustomPlot);
        ui->qCustomPlot->plotLayout()->addElement(0,1,colorScale);
        colorScale->setDataRange(QCPRange(0,300));
        colorMap->setColorScale(colorScale);

        //以下尝试自己设置色彩梯度
        QCPColorGradient neo;
        neo.setLevelCount(350);
        neo.setColorStopAt(0,Qt::white);
        neo.setColorStopAt(0.3,QColor(0,26,123,255));
        neo.setColorStopAt(0.4,QColor(56,142,113,255));
        neo.setColorStopAt(0.5,QColor(162,204,58,255));
        neo.setColorStopAt(0.57,Qt::yellow);
        neo.setColorStopAt(0.6,QColor(239,210,31,255));
        neo.setColorStopAt(0.7,QColor(208,126,89,255));
        neo.setColorStopAt(0.8,QColor(203,114,48,255));
        neo.setColorStopAt(0.9,QColor(177,73,18,255));
        neo.setColorStopAt(0.95,QColor(162,40,19,255));
        neo.setColorStopAt(1,Qt::black);
        neo.setColorInterpolation(neo.ciRGB);
        colorMap->setGradient(neo);

       QCPMarginGroup *marginGroup = new QCPMarginGroup(ui->qCustomPlot);
       ui->qCustomPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);


       //状态栏显示当前的a,nx,ny
       ui->statusBar->showMessage(QString("当前的a: %1  , 当前绘图区域大小:  %2 * %3  , IDW寻找方格边长:  %4")
                                  .arg(a)
                                  .arg(nx)
                                  .arg(ny)
                                  .arg(2*radius+1)
                                  ,0);


}


MainWindow::~MainWindow()
{
    delete ui;
}

