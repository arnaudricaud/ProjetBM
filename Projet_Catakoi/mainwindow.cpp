#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cam=new VideoCapture(0);
    //int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    //int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);


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
    PointMin->y=1000;
    PointMax->y=0;
    PointPrev->y=0;
    PointPrev2->y=0;
    PointMin->x=0;
    PointMax->x=0;
    PointPrev->x=0;
    PointPrev2->x=0;
    start =false;
    go =false;
    angle = false;
    on =false;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::update(){

    if (cam->isOpened()) {
        Mat image;
        Rect resultRect;
        Rect resultRect2;
        Rect resultRect3;
        if (cam->read(image)) {   // Capture a frame
            ui->camFrame->setText("");
            // Flip to get a mirror effect
           // flip(image,image,1);
            float newsize = (ui->centralWidget->width())/5;
            cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
            flip(image,image,1);
            //int newheight = (ui->centralWidget->width())/4;;
            //int newwidth = (newheight*1.33);
            //cv::resize(image, image,Size(newwidth,newheight), 0, 0);
            //cv::resize(image, image,Size(), 0.1, 0.1);
            // Invert Blue and Red color channels
            cvtColor(image,image,CV_BGR2RGB);
            if(go){
            ui->labelZero->setVisible(true);
            ui->labelCent->setVisible(true);
            resultRect=Rect(10,50,120,1);
            // Draw red rectangle on the frame
            rectangle(image,resultRect,Scalar( 255, 0, 0),2,8,0);
            resultRect2=Rect(10,120,120,1);
            rectangle(image,resultRect2,Scalar( 255, 0, 0),2,8,0);
            ui->labelInstruction->setText("Déterminez la puissance");
            ui->labelInstruction->setVisible(true);
            }
            if(angle){
            float x =(PointMin->x*150/580);
            resultRect=Rect(140-x,30,2,20);
            rectangle(image,resultRect,Scalar( 255, 0, 0),2,8,0);
            /*resultRect2=Rect(x-25,30,1,20);
            rectangle(image,resultRect2,Scalar( 255, 0, 0),2,8,0);
            resultRect3=Rect(x+25,30,1,20);
            rectangle(image,resultRect3,Scalar( 255, 0, 0),2,8,0);*/
            }
            // Convert to Qt image
            QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
            // Display on label
            ui->camFrame->setPixmap(QPixmap::fromImage(img));
            // Resize the label to fit the image
            ui->camFrame->resize(ui->camFrame->pixmap()->size());

        }
        else {
            ui->camFrame->setText("Error capturing the frame");
        }

    }
}

void MainWindow::tracking(){
    if (cam->isOpened()) {
        Mat image;
        Mat resultImage;
        float pourPui;
        float pourAngle;
        if (cam->read(image)){ //image est notre image de calibrage.
            //Il faut appliquer des traitements pour trouver le point
            //qui va permettre de découper notre image en 4 imagettes.

               // Motif que l'on recherche
                Mat templateImage = imread("../Projet_Catakoi/SnapGomme.JPG");
                int result_cols =  image.cols - templateImage.cols + 1;
                int result_rows = image.rows - templateImage.rows + 1;
                resultImage.create( result_cols, result_rows, CV_32FC1 );

                // Do the Matching between the frame and the templateImage
                matchTemplate( image, templateImage, resultImage, TM_CCORR_NORMED );
                // Localize the best match with minMaxLoc
                double minVal; double maxVal; Point minLoc; Point maxLoc;
                minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
               // cout<<"maxVal : "<<maxVal<<"minVal : "<<minVal<<endl;
                if (maxVal>0.965){
                //namedWindow("Image de calibrage",1);
                //imshow("Image de calibrage", image);
                }
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
                    }

                    }
        }
    }

}

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        go=true;
        connect(timer2, SIGNAL(timeout()), this, SLOT(tracking()));
        timer2->start(1000);
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
    PointMin->y=1000;
    PointMax->y=0;
    PointPrev->y=0;
    PointPrev2->y=0;
    PointMin->x=0;
    PointMax->x=0;
    PointPrev->x=0;
    PointPrev2->x=0;
    start =false;
    go =false;
    angle = false;
    on =false;
}
