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
    int width=cam->get(CV_CAP_PROP_FRAME_WIDTH);
    int height=cam->get(CV_CAP_PROP_FRAME_HEIGHT);


    connect(ui->myGLWidget, SIGNAL(zoomChanged(int)), ui->zoomSlider, SLOT(setValue(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(50);


PointMin->y=1000;
PointMax->y=0;
PointPrev->y=0;
start =false;
go =false;
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
        if (cam->read(image)) {   // Capture a frame
            ui->camFrame->setText("");
            // Flip to get a mirror effect
           // flip(image,image,1);
            float newsize = (ui->centralWidget->width())/5;
            cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
            // Invert Blue and Red color channels
            cvtColor(image,image,CV_BGR2RGB);
            // Convert to Qt image
            QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
            // Display on label
            ui->camFrame->setPixmap(QPixmap::fromImage(img));
            // Resize the label to fit the image
            ui->camFrame->resize(ui->camFrame->pixmap()->size());
            if(go){
            resultRect=Rect(100,100,100,100);
            // Draw green rectangle on the frame
            rectangle(image,resultRect,Scalar( 255, 0, 0),2,8,0);
            }
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
                namedWindow("Image de calibrage",1);
                imshow("Image de calibrage", image);
                }
                cout<<maxLoc.x<<" et "<<maxLoc.y<<endl;
                if (maxLoc.y<100){
                    cout<<"Lancement tracking"<<endl;
                    if(maxLoc.y<PointMin->y){
                    PointMin->y = maxLoc.y;
                    start = true;
                    cout<<"PointMin Y :"<<PointMin->y<<endl;
                    }
                }
                if(maxLoc.y>PointPrev->y ){
                    cout<<"yes"<<endl;
                    if (start){
                     PointPrev->y=maxLoc.y;
                          float dist = PointPrev->y-PointMin->y;
                     cout<<"Distance calculee -> "<<dist<<endl;
                    }
                }

        }
    }

}

void MainWindow::on_pushButton_clicked()
{
    go=true;
    cout<<"go true"<<endl;

   // connect(timer2, SIGNAL(timeout()), this, SLOT(tracking()));
   // timer2->start(1000);
}
