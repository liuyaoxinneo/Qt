#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "doppler_net.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//public slots:
//     void do_device_capture_event(const QByteArray &data);

public slots:
    void timeupdate();
    void angleupdate();
    void material();

private:
    Ui::MainWindow *ui;
    Doppler_net net;
};

#endif // MAINWINDOW_H
