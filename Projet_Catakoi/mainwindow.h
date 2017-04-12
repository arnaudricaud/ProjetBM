#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"


using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();


private:
    Ui::MainWindow *ui;
    VideoCapture * cam;
    QTimer *timer = new QTimer(this);

};

#endif // MAINWINDOW_H
