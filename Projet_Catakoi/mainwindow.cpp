#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include <QPixmap>
#include <QDebug>
#include <GL/glu.h>
#include <QtOpenGL>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cam=new VideoCapture(0);

    if(!cam->isOpened())  // check if we succeeded
    {
        cerr<<"Error openning the default camera"<<endl;

    }
    //On redimensione la caméra
    int frameWidth=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight=cam->get(CV_CAP_PROP_FRAME_HEIGHT);
    frameWidth=frameWidth/2;
    frameHeight=frameHeight/2;
    cam->set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    cam->set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

    templateRect= new Rect((frameWidth-templateWidth)/2,(frameHeight-templateHeight)/2, templateWidth,templateHeight);

    connect(ui->myGLWidget, SIGNAL(zoomChanged(int)), ui->zoomSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(angleCatapulteChanged(int)), ui->SliderAngleCatapulte, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(angleBrasChanged(int)), ui->SliderAngleBras, SLOT(setValue(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(50);
    ui->labelZero->setVisible(false);
    ui->labelCent->setVisible(false);
    ui->labelPuissance->setVisible(false);
    ui->labelAngle->setVisible(false);
    ui->labelInstruction->setVisible(false);

    prevY = 15;
    start =false;
    go =false;
    angle = false;
    on =false;
    track=true;
    lanceBall=false;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::update(){
        if (cam->read(image)) {   // Capture a frame

           flip(image,image,1);
           templateImage = Mat(image, *templateRect).clone();
           if(!go){
                 rectangle(image, *templateRect, Scalar(0,0,255),2,8,0);
           }
           float newsize = (ui->centralWidget->width())/5;
           cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
           cvtColor(image,image,CV_BGR2RGB);
           QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
           ui->camFrame->setPixmap(QPixmap::fromImage(img));
          }
    }


void MainWindow::tracking(){


      //int xInit = templateRect->x; //130
      //int yInit = templateRect->y; //90

    if (cam->read(image)) // get a new frame from camera
    {
        Rect resultRect;
        // vertical flip of the image
        flip(image,image,1);

        // Do the Matching between the frame and the templateImage
        matchTemplate( image, matchImage, resultImage, TM_CCORR_NORMED );
        // Localize the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        // Save the location fo the matched rect
        resultRect=Rect(maxLoc.x,maxLoc.y,templateWidth,templateHeight);
        // Show the result
        Mat normResultImage;
        // Normalize values
        normalize(resultImage,normResultImage,1,0,NORM_MINMAX);
        // Return to RGB to plot the max in red
        cvtColor(normResultImage,normResultImage,CV_GRAY2RGB);
        // Draw a red square
        rectangle(normResultImage,Rect(maxLoc.x,maxLoc.y,3,3),Scalar( 0, 0, 1),2,8,0);
        // Draw green rectangle on the frame
        rectangle(image,resultRect,Scalar( 0, 255, 0),2,8,0);
        // Display the image

        float newsize = (ui->centralWidget->width())/5;
        cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
        cvtColor(image,image,CV_BGR2RGB);
        QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        ui->camFrame->setPixmap(QPixmap::fromImage(img));


       // cout<<"yPrev "<<yPrev<<" et now"<<resultRect.y<<endl;
      if(!lanceBall){
        if (resultRect.y>yPrev+20)
                {
                    lanceBall=true;
                    distance=yPrev-yMin;
                    cout<<yPrev<<" et "<<yMin<<endl;
                    cout<<"distance :"<<distance<<endl;
                  // timer2->stop();
                }
        else{
            yPrev=resultRect.y;
            xPrev=resultRect.x;
            angleCatapulte=xPrev-xMin;
            distance=yPrev-yMin;
            ui->SliderAngleCatapulte->setValue(angleCatapulte);
            ui->SliderAngleBras->setValue(distance);
            if (yPrev<yMin){
                yMin=yPrev;
            }
           // cout<<yPrev<<endl;
        }
        if (lanceBall){
            // MyGLWidget::launchBall();
        }
    }



  /*

                resultRect=Rect(maxLoc.x,maxLoc.y,50,50);
                rectangle(resultImage,resultRect,Scalar( 0, 255, 0),2,8,0);
                cout<<maxLoc.x<<" et "<<maxLoc.y<<endl;
                if (maxLoc.y<150 && !angle && go){
                    ui->labelPuissance->setVisible(true);
                   // ui->labelAngle->setVisible(true);
                    cout<<"Lancement tracking"<<endl;
                    if(maxLoc.y<PointMin->y){
                    PointMin->y = maxLoc.y;
                    start = true;
                    cout<<"PointMin Y :"<<PointMin->y<<endl;
                    }
                }
                if(maxLoc.y>PointPrev->y && !angle && go){
                    cout<<"yes"<<endl;
                    if (start){
                     PointPrev2->y=PointPrev->y;
                     PointPrev->y=maxLoc.y;
                     PointPrev->x=maxLoc.x;
                          float dist = PointPrev->y-PointMin->y;
                     cout<<"Distance calculee -> "<<dist<<endl;
                     pourPui =floor((100*dist)/250);
                     if(pourPui>100){
                         pourPui=100;
                         start=false;
                         angle=true;
                     }
                     ui->labelPuissance->setText(QString::number(pourPui)+"%");
                     cout<<"point 2 : "<<PointPrev2->y<<" et point 1 : "<<PointPrev->y<<PointPrev->x<<endl;
                    }
                }
                if(maxLoc.y<PointPrev2->y+8 && maxLoc.y>PointPrev2->y-8 &&!angle){
                     cout<<"OK STOP 1"<<endl;
                     if(maxLoc.y<PointPrev->y+5 && maxLoc.y>PointPrev->y-5 && !angle){
                    cout<<"OK STOP 2"<<endl;
                    PointMin->x=PointPrev->x;
                    //start=false;
                    angle=true;
                    go=false;
                    //On crée un point de référence pour l'angle
                    ui->labelZero->setVisible(false);
                    ui->labelCent->setVisible(false);
                    ui->labelInstruction->setText("Déterminez l'angle");
                    ui->labelAngle->setVisible(true);
                        }
                }
                    if (angle && !go){

                     cout<<"Lancement tracking angle"<<endl;
                     PointMax->x = maxLoc.x;
                     PointPrev2->x=PointPrev->x;
                     PointPrev->x=maxLoc.x;
                          float dist = PointPrev->x-PointMin->x;
                     cout<<"Distance calculee -> "<<dist<<endl;
                    if (dist>0){
                     if (dist>90){
                         pourAngle=45;
                     }else if (dist>80){
                         pourAngle=40;
                     }else if (dist>70){
                         pourAngle=35;
                     }else if  (dist>60){
                         pourAngle=30;
                     }else if (dist>50){
                         pourAngle=25;
                     }else if (dist>40){
                         pourAngle=20;
                     }else if (dist>30){
                         pourAngle=15;
                     }else if (dist>20){
                         pourAngle=10;
                     }else if (dist>10){
                         pourAngle=5;
                     }else{
                         pourAngle=0;
                     }
                     ui->labelAngle->setText("-"+QString::number(pourAngle)+"°");
                    }else{
                        dist = -dist;
                        if (dist>90){
                            pourAngle=45;
                        }else if (dist>80){
                            pourAngle=40;
                        }else if (dist>70){
                            pourAngle=35;
                        }else if  (dist>60){
                            pourAngle=30;
                        }else if (dist>50){
                            pourAngle=25;
                        }else if (dist>40){
                            pourAngle=20;
                        }else if (dist>30){
                            pourAngle=15;
                        }else if (dist>20){
                            pourAngle=10;
                        }else if (dist>10){
                            pourAngle=5;
                        }else{pourAngle=0;}
                        ui->labelAngle->setText("+"+QString::number(pourAngle)+"°");
                    }*/
    }

}

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        go=true;
        // Create the matchTemplate image result
            // to store the matchTemplate result
        int result_cols =  image.cols - templateImage.cols + 1;
        int result_rows = image.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );
        matchImage=templateImage;
        //on arrete l'affichage simple
        timer->stop();
        connect(timer2, SIGNAL(timeout()), this, SLOT(tracking()));
        timer2->start(50);
    }else{
        go=false;
        timer2->stop();
        reset();
    }
}

// reset() permet de remettre à zéro toutes les valeurs des points et de cacher les différents labels
void MainWindow::reset(){
    ui->labelZero->setVisible(false);
    ui->labelCent->setVisible(false);
    ui->labelPuissance->setVisible(false);
    ui->labelAngle->setVisible(false);
    ui->labelInstruction->setVisible(false);
    timer->stop();
    timer2->stop();
    start =false;
    go =false;
    angle = false;
    on =false;
    initialX=0;
    initialY=0;
    lanceBall=false;
    templateImage.deallocate();
}
