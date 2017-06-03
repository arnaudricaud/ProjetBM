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
    void reset();
    int getInitialX() {return initialX;}
    int getInitialY() {return initialY;}
    int getPuissance() {return distance;}
    void setxPostion(int x);
    void setyPostion(int y);

private slots:
    void update();
    void tracking();
    void on_checkBox_clicked();



private:
    Ui::MainWindow *ui;
    VideoCapture * cam;
    QTimer *timer = new QTimer(this);
    QTimer *timer2 = new QTimer(this);

    bool  start;
    bool angle;
    bool go;
    bool on;
    bool track;
    bool lanceBall;
    int templateWidth = 60;
    int templateHeight = 60;
    cv::Rect *templateRect;
    cv::Mat templateImage;
    cv::Mat matchImage;
    cv::Mat resultImage;
    cv::Mat image;
    int initialX;
    int initialY;
    int prevY;
    int xPosition=0;
    int yPosition=0;
    int yPrev=90;
    int yMin=90;
    int xPrev=130;
    int xMin=130;
    int distance=0;
    int angleCatapulte=0;
//int angleCatapulte;
//QLabel *labelZero=new QLabel(this);
};

#endif // MAINWINDOW_H
