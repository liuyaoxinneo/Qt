#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

int n=0;

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
    ui->label->setText("位置");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_device_capture_event(const QByteArray &data)
{
//此处加实时更新的测量值

//    读取编码器信息
    n=n+1;
    memcpy(&encoder,(data.data()+net.cur_group_point_qty()+4*4),4);
    qDebug()<<n<<encoder;
    ui->encoder->display(encoder);
}
