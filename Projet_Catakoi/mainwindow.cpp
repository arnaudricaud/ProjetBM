#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>


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


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;
}
void MainWindow::update(){

    if (cam->isOpened()) {
        Mat image;
        if (cam->read(image)) {   // Capture a frame
            ui->camFrame->setText("");
            // Flip to get a mirror effect
            flip(image,image,1);
            //float newsize = (ui->centralWidget->width())/5;
            //cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
            // Invert Blue and Red color channels
            cvtColor(image,image,CV_BGR2RGB);
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
