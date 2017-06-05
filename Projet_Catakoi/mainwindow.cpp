#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include "gamedialog.h"
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
    frameWidth=frameWidth/4;
    frameHeight=frameHeight/4;
    cam->set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
    cam->set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

    templateRect= new Rect((frameWidth-templateWidth)/2,-20+(frameHeight-templateHeight)/2, templateWidth,templateHeight);

    //connect(ui->myGLWidget, SIGNAL(zoomChanged(int)), ui->zoomSlider, SLOT(setValue(int)));
    //connect(ui->myGLWidget, SIGNAL(angleCatapulteChanged(int)), ui->SliderAngleCatapulte, SLOT(setValue(int)));
    //connect(ui->myGLWidget, SIGNAL(angleBrasChanged(int)), ui->SliderAngleBras, SLOT(setValue(int)));
    //SCORE
    connect(ui->myGLWidget, SIGNAL(changeScore(int)),this, SLOT(setScore(int)));
    //TIMERS
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //connect(timer2, SIGNAL(timeout()), this, SLOT(tracking()));
    connect(chronoTotal, SIGNAL(timeout()),this,SLOT(chronoRefresh()));
    connect(chronoCible, SIGNAL(timeout()),this,SLOT(chronoRefresh2()));
    //PARAM CATAPULTE ET LANCÉ
    connect(this, SIGNAL(launchBall()),ui->myGLWidget, SLOT(launchBall()));
    connect(this, SIGNAL(setAngleCatapulte(int)),ui->myGLWidget, SLOT(setAngleCatapulte(int)));
    connect(this, SIGNAL(setAngleBras(int)),ui->myGLWidget, SLOT(setAngleBras(int)));
    connect(this, SIGNAL(changePuissance(int)),ui->myGLWidget, SLOT(setPuissance(int)));
    connect(this, SIGNAL(changeLevel(int)),ui->myGLWidget, SLOT(setLevel(int)));
   // connect(this, SIGNAL(debutGame()), this, SLOT(demarrer()));

    timer->start(100);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);

    start =false;
    go =false;
    track=true;
    lanceBall=false;
    ui->checkBox->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(true);
    ui->label_4->setVisible(true);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cam;

}
void MainWindow::update(){

    if(!go){
        if (cam->read(image)) {   // Capture a frame
           flip(image,image,1);
           templateImage = Mat(image, *templateRect).clone();
           rectangle(image, *templateRect, Scalar(0,0,255),2,8,0);
           //float newsize = (ui->centralWidget->width())/5;
           //cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
           cvtColor(image,image,CV_BGR2RGB);
           QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
           ui->camFrame->setPixmap(QPixmap::fromImage(img));
         }
    } else {
        if (cam->read(image)) // get a new frame from camera
        {
            Rect resultRect;
            double minVal; double maxVal; Point minLoc; Point maxLoc;
            maxVal=0.20;
            // vertical flip of the image
            flip(image,image,1);
            // Do the Matching between the frame and the templateImage
            matchTemplate(image, matchImage, resultImage, TM_CCORR_NORMED);
            // Localize the best match with minMaxLoc
            minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
            // Save the location fo the matched rect
            resultRect=Rect(maxLoc.x,maxLoc.y,templateWidth,templateHeight);
            rectangle(image,resultRect,Scalar( 0, 255, 0),2,8,0);
            // Display the image
            //float newsize = (ui->centralWidget->width())/5;
            //cv::resize(image, image, Size(newsize, newsize), 0, 0, INTER_LINEAR);
            cvtColor(image,image,CV_BGR2RGB);
            QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
            ui->camFrame->setPixmap(QPixmap::fromImage(img));
            if(!lanceBall){
                if (resultRect.y>yPrev+12){
                    cv::Mat img;
                    image=img;
                    lanceBall=true;
                    distance=yPrev-yMin;
                    distance=(100*distance)/80;
                    setPuissance(distance);
                    ui->progressBar->setValue(distance);
                }else{
                    yPrev=resultRect.y;
                    xPrev=resultRect.x;
                    angleCatapulte=xPrev-xMin;
                    distance = yPrev - yMin;
                    distance=(100*distance)/80;
                    ui->progressBar->setValue(distance);
                    setAngleCatapulte(angleCatapulte);
                    angleBras=(30*distance)/100;
                    setAngleBras(angleBras);
                    if (yPrev<yMin){
                        yMin=yPrev;
                    }
                }
                if (lanceBall){
                    emit changePuissance(distance);
                    chronoCible->stop();
                    emit launchBall();
                    calculPartie();
                    lanceBall = false;
                    count=count+1;
                    reset();
                }
            }
        }
    }
}

void MainWindow::calculPartie(){
    if (countGame==4)
    {
        //ui->labelCible->setText("Fin de la partie !");
        ui->labelCibleR->setText("Fin partie !");
        chronoTotal->stop();
    }else{
        chronoCible->start(1000);//lancement du chrono de la cible
        countGame++;
        count=0;
        QString nb= QString::number(10-countGame);
       // ui->labelCible->setText(QString("nombre de cibles restantes : "+nb));
        ui->labelCibleR->setText(nb);
    }
}
void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        // Create the matchTemplate image result
        // to store the matchTemplate result
        int result_cols =  image.cols - templateImage.cols + 1;
        int result_rows = image.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );
        matchImage=templateImage;
        ui->progressBar->setVisible(true);
        go=true;
    }else{
        reset();
    }
}


// reset() permet de remettre à zéro toutes les valeurs des points et de cacher les différents labels
void MainWindow::reset(){
    yMin = 200;
    yPrev = yMin;
    ui->progressBar->setVisible(false);
    ui->checkBox->setChecked(false);
    start =false;
    go =false;
    lanceBall=false;

}

int MainWindow::getPuissance(){
    return distance;
}

void MainWindow::setPuissance(int dist){
    distance=dist;
}

void MainWindow::on_boutonPlay_clicked()
{
    ui->checkBox->setVisible(true);
    gameDialog x;
    x.setModal(true);
    x.exec();
    //x.getName();
    level=x.getDifficulty();
    nomJoueur = x.getName();
    cout<<"difficulte : "<< level<<endl;
   // cout<<" et nom : "<<nom<<endl;
    qDebug()<<nomJoueur;
    chronoTotal->start(1000);
    timeCur->currentTime();
    cout<<"temps pris : "<<timeCur<<endl;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
        timer->start(50);
    }else if (event->key() == Qt::Key_0){
        go=true;
        int result_cols =  image.cols - templateImage.cols + 1;
        int result_rows = image.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );
        matchImage=templateImage;
    }
}

void MainWindow::chronoRefresh()
{
    //Calcul du chronometre pour la partie total
    QString temps = ui->labelChronoTot->text();
    QStringList list = temps.split(":");
    QString minute = list.join(QString(":"));
    //minute=minute.mid(12,1);
    list.removeFirst();
    QString secon = list.join(QString(":"));
    int tps=(minute.toInt())*60+secon.toInt();
    if (tps!=-1)
    {
       tps++;
       int min = tps/60;
       int seconde = tps - min*60;
       ui->labelChronoTot->setText(QString::number(min)+":"+QString::number(seconde));
    }
    else
    {
        chronoTotal->stop();
    }
}

void MainWindow::chronoRefresh2()
{
    //Calcul du chronometre pour un lancé
    QString temps = ui->labelChronoCible->text();
    QStringList list = temps.split(":");
    QString minute = list.join(QString(":"));
    //minute=minute.mid(12,1);
    list.removeFirst();
    QString secon = list.join(QString(":"));
    int tps=minute.toInt()*60+secon.toInt();
    if (tps!=-1)
    {

       tps++;
       int min = tps/60;
       int seconde = tps - min*60;
       ui->labelChronoCible->setText(QString::number(min)+":"+QString::number(seconde));
    }
    else
    {
        chronoCible->stop();
    }
}
void MainWindow::setScore(int sco){
    if(sco<5){
        score=score+10;
    }else if (sco<10){
        score=score+9;
    }else if (sco<15){
        score=score+8;
    }else if (sco<20){
        score=score+7;
    }else if (sco<25){
        score=score+6;
    }else if (sco<30){
        score=score+5;
    }else if (sco<35){
        score=score+4;
    }else if (sco<40){
        score=score+3;
    }else if (sco<45){
        score=score+2;
    }else if (sco<50){
        score=score+1;
    }else{
        score=score+0;
    }
    qDebug()<<"Score : "<<score;
}

