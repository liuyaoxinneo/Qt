/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *qCustomPlot;
    QSpinBox *data_spinBox;
    QLabel *label;
    QCustomPlot *qCustomPlot2;
    QSpinBox *angle_spinBox;
    QLabel *label1;
    QDoubleSpinBox *gainnumber;
    QLabel *gainlabel;
    QLCDNumber *vol;
    QLabel *vollabel;
    QComboBox *materialbox;
    QLabel *materials;
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(742, 709);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        qCustomPlot = new QCustomPlot(centralWidget);
        qCustomPlot->setObjectName(QStringLiteral("qCustomPlot"));
        qCustomPlot->setGeometry(QRect(10, 210, 591, 441));
        data_spinBox = new QSpinBox(qCustomPlot);
        data_spinBox->setObjectName(QStringLiteral("data_spinBox"));
        data_spinBox->setGeometry(QRect(480, 0, 61, 22));
        label = new QLabel(qCustomPlot);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(447, 5, 54, 12));
        qCustomPlot2 = new QCustomPlot(centralWidget);
        qCustomPlot2->setObjectName(QStringLiteral("qCustomPlot2"));
        qCustomPlot2->setGeometry(QRect(10, 0, 591, 201));
        angle_spinBox = new QSpinBox(qCustomPlot2);
        angle_spinBox->setObjectName(QStringLiteral("angle_spinBox"));
        angle_spinBox->setGeometry(QRect(520, 10, 61, 22));
        label1 = new QLabel(qCustomPlot2);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setGeometry(QRect(480, 10, 31, 21));
        gainnumber = new QDoubleSpinBox(centralWidget);
        gainnumber->setObjectName(QStringLiteral("gainnumber"));
        gainnumber->setGeometry(QRect(610, 40, 121, 31));
        gainlabel = new QLabel(centralWidget);
        gainlabel->setObjectName(QStringLiteral("gainlabel"));
        gainlabel->setGeometry(QRect(610, 20, 81, 20));
        vol = new QLCDNumber(centralWidget);
        vol->setObjectName(QStringLiteral("vol"));
        vol->setGeometry(QRect(610, 100, 121, 31));
        vollabel = new QLabel(centralWidget);
        vollabel->setObjectName(QStringLiteral("vollabel"));
        vollabel->setGeometry(QRect(610, 80, 91, 16));
        materialbox = new QComboBox(centralWidget);
        materialbox->setObjectName(QStringLiteral("materialbox"));
        materialbox->setGeometry(QRect(610, 160, 121, 31));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(materialbox->sizePolicy().hasHeightForWidth());
        materialbox->setSizePolicy(sizePolicy1);
        materials = new QLabel(centralWidget);
        materials->setObjectName(QStringLiteral("materials"));
        materials->setGeometry(QRect(610, 140, 121, 16));
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 742, 23));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "data", Q_NULLPTR));
        label1->setText(QApplication::translate("MainWindow", "angle", Q_NULLPTR));
        gainlabel->setText(QApplication::translate("MainWindow", "\345\242\236\347\233\212\357\274\210dB\357\274\211", Q_NULLPTR));
        vollabel->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\345\243\260\351\200\237\357\274\210m/s\357\274\211", Q_NULLPTR));
        materialbox->clear();
        materialbox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\351\223\201", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\351\223\235", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\346\234\250", Q_NULLPTR)
        );
        materialbox->setCurrentText(QApplication::translate("MainWindow", "\351\223\201", Q_NULLPTR));
        materials->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
