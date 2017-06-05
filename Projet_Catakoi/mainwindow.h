#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include<QLabel>
#include"gamedialog.h"
#include <QTime>


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
    int getPuissance();
    void setPuissance(int dist);
    void setxPostion(int x);
    void setyPostion(int y);
protected:
    void keyPressEvent(QKeyEvent *);
    void calculPartie();

private slots:
    void update();
    //void tracking();
    //void transition();
    void on_checkBox_clicked();
    void on_boutonPlay_clicked();
    void chronoRefresh();
    void chronoRefresh2();
    void setScore(int sco);

signals:
void launchBall();
void setAngleCatapulte(int angleCatapulte);
void setAngleBras(int angleBras);
void changePuissance(int puissance);
void changeLevel(int level);
void debutGame();



private:

    void saveHighScore();
    void loadHighScore();

    Ui::MainWindow *ui;

    VideoCapture * cam;
    QTimer *timer = new QTimer(this);
    QTimer *timer2 = new QTimer(this);
    QTimer *timer3 = new QTimer(this);
    QTimer *chronoTotal = new QTimer(this);
    QTimer *chronoCible = new QTimer(this);
    QTime *temps = new QTime(0,0,0,0);
    QTime *chronometre = new QTime(0,0,0,0);
    QString timeRef;
    QString timeRefC;
    bool go;
    bool lanceBall;
    int templateWidth = 30;
    int templateHeight = 30;
    cv::Rect *templateRect;
    cv::Mat templateImage;
    cv::Mat matchImage;
    cv::Mat resultImage;
    cv::Mat image;
    int yPrev=90;
    int yMin=20;
    int xPrev=130;
    int xMin=60;
    int distance;
    int puissance;
    int angleCatapulte=0;
    int angleBras=0;
    int countGame=0;
    int level=3;
    int score = 0;
    QString nomJoueur;

};

#endif // MAINWINDOW_H
