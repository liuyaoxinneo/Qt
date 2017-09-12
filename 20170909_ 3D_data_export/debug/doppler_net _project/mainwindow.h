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
public slots:
     void first_data_output(const QByteArray &data);
     void rest_data_output(const QByteArray &data);
     void encoder_value(const QByteArray &data);

signals:
     void encoder_value_change();

private:
    Ui::MainWindow *ui;
    Doppler_net net;
    unsigned char encoder;
};

#endif // MAINWINDOW_H
