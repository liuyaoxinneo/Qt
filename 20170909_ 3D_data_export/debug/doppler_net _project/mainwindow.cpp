#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "qfile.h"

//定义全局变量
unsigned char valuebuf;
QString name;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net.tcp_connect("192.168.1.215");
    net.udp_connect();//与仪器通过UDP方式连接
    net.enable_data_transfer(true);//为true则启动数据传输

    connect(&net,SIGNAL(capture_event(QByteArray)),this,SLOT(first_data_output(QByteArray)));
    //连接信号和槽
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::first_data_output(const QByteArray &data)
{

//在此处解除信号capture_event()和槽的连接，从而只显示31条beam的各点信息1次
     disconnect(&net,SIGNAL(capture_event(QByteArray)),this,SLOT(first_data_output(QByteArray)));
 //读取编码器和波形的信息
 //读编码器信息
    memcpy(&encoder,(data.data()+4*4),1);
    qDebug()<<"start at:"<<encoder;

    valuebuf=encoder;

 //读18755点强度值信息
      unsigned char buffer[31][605];
      unsigned char txt[18755];
      int i,j;
      for(j=0;j<31;j++)
      {
          for(i=0;i<605;i++)
          {
          memcpy(&buffer[j][i],(data.data()+j*637+i),1);
          txt[605*j+i]=buffer[j][i];//将所有有用的18755的点的数据放入一维数组txt[]中
          }
      }

  //写数据文件部分
      QFile file(encoder+".txt");
      if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
          return;
      QTextStream out(&file);
      for(int p=0;p<18755;p++)
      {
          out<<txt[p]<<" "<<"\n";
      }
      file.close();
      qDebug()<<"first data.txt has been written!";

      connect(&net,SIGNAL(capture_event(QByteArray)),this,SLOT(encoder_value()));
}

void MainWindow::encoder_value(const QByteArray &data)
{
    memcpy(&encoder,(data.data()+4*4),1);
    if(valuebuf!=encoder)
    {
        valuebuf=encoder;
        emit encoder_value_change();
    }
    connect(this,SIGNAL(encoder_value_change()),this,SLOT(rest_data_output(QByteArray)));
}

void MainWindow::rest_data_output(const QByteArray &data)
{
    unsigned char buffer[31][605];
    unsigned char txt[18755];
    int i,j;
    for(j=0;j<31;j++)
    {
        for(i=0;i<605;i++)
        {
        memcpy(&buffer[j][i],(data.data()+j*637+i),1);
        txt[605*j+i]=buffer[j][i];//将所有有用的18755的点的数据放入一维数组txt[]中
        }
    }

//写数据文件部分
    QFile file(encoder+".txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int p=0;p<18755;p++)
    {
        out<<txt[p]<<" "<<"\n";
    }
    file.close();
    qDebug()<<"data.txt at "<<encoder<<" has been written!";
}

