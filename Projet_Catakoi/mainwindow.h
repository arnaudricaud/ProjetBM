#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include<QLabel>


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
    void tracking();


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    VideoCapture * cam;
    QTimer *timer = new QTimer(this);
    QTimer *timer2 = new QTimer(this);
    Point *PointMin = new Point();
    Point *PointMax = new Point();
    Point *PointPrev = new Point();
    bool  start;
    bool go;
//QLabel *labelZero=new QLabel(this);
};

#endif // MAINWINDOW_H
