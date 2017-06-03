#include "mainwindow.h"
#include <QApplication>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <QSettings>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSettings settings("RicaudValeille", "ProjetCatakoi");
    settings.setValue("test", true);
    return a.exec();
}
