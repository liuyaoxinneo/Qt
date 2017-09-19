#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include"qfile.h"

int n=0;
int encoderbuf;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net.tcp_connect("192.168.1.215");
    net.udp_connect();//与仪器通过UDP方式连接
    net.enable_data_transfer(true);//为true则启动数据传输

    connect(&net,SIGNAL(capture_event(QByteArray)),this,SLOT(do_device_capture_event(QByteArray)));
    //连接信号和槽
    ui->label1->setText("上一位置");
    ui->label2->setText("下一位置");
    ui->label3->setText("位移");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::do_device_capture_event(const QByteArray &data)
{
//    读取编码器信息
//    n=n+1;
    memcpy(&encoder,(data.data()+net.cur_group_point_qty()+4*4),4);
//    qDebug()<<n<<encoder;
    if(encoder!=encoderbuf)
    {
        ui->encoder0->display(encoderbuf);
        ui->encoder1->display(encoder);
        ui->encoder->display(encoder-encoderbuf);
        encoderbuf=encoder;

        if(encoder>0)
        {
        QString name=QString("data%1.txt")
                .arg(encoder);
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

            QFile file(name);
            if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
                return;
            QTextStream out(&file);
            for(int p=0;p<18755;p++)
            {
                out<<txt[p]<<" "<<"\n";
            }
            file.close();
            qDebug()<<name<<" has been written!";
        }
        }
    }
}
