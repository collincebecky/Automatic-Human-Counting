#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qputenv("OPENCV_DNN_OPENCL_ALLOW_ALL_DEVICES", 1);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qRegisterMetaType<QVector<int> >("QVector<int>");

    return a.exec();
}
