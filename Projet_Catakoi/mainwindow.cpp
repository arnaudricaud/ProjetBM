#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myglwidget.h"
#include "gamedialog.h"
#include <QPixmap>
#include <QDebug>
#include <GL/glu.h>
#include <QtOpenGL>
#include <QString>

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

    //FOCUS FENETRE
    setFocusPolicy(Qt::StrongFocus);

    //SCORE
    loadHighScore();
    connect(ui->myGLWidget, SIGNAL(changeScore(int)),this, SLOT(setScore(int)));
    //TIMERS
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //CHRONOMETRE
    connect(chronoTotal, SIGNAL(timeout()),this,SLOT(chronoRefresh()));
   // connect(chronoCible, SIGNAL(timeout()),this,SLOT(chronoRefresh2()));
    connect(chronoCible,SIGNAL(timeout()), this, SLOT(chronoRefresh2()));
    //PARAM CATAPULTE ET LANCÉ
    connect(this, SIGNAL(launchBall()),ui->myGLWidget, SLOT(launchBall()));
    connect(this, SIGNAL(setAngleCatapulte(int)),ui->myGLWidget, SLOT(setAngleCatapulte(int)));
    connect(this, SIGNAL(setAngleBras(int)),ui->myGLWidget, SLOT(setAngleBras(int)));
    connect(this, SIGNAL(changePuissance(int)),ui->myGLWidget, SLOT(setPuissance(int)));
    connect(this, SIGNAL(changeLevel(int)),ui->myGLWidget, SLOT(setLevel(int)));


    timer->start(100);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);

    go =false;
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
                    reset();
                    chronoCible->stop();
                }
            }
        }
    }
}

void MainWindow::calculPartie(){
    if (countGame==9)
    {
        //ui->labelCible->setText("Fin de la partie !");
        ui->labelCibleR->setText("Fin partie !");
        ui->labelCibleJ->setText(QString::number(10));
        chronoTotal->stop();
        displayHighScore();
        saveHighScore();
        ui->checkBox->setVisible(false);
    }else{
        chronoCible->start(1000);//lancement du chrono de la cible
        countGame++;
        QString nb= QString::number(10-countGame);
       // ui->labelCible->setText(QString("nombre de cibles restantes : "+nb));
        ui->labelCibleR->setText(nb);
        ui->labelCibleJ->setText(QString::number(countGame));
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
        timeRefC = (QTime::currentTime().toString("hh:mm:ss"));
        chronoCible->start(1000);
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
    int result = x.exec();
    if (result == 1){
        level=x.getDifficulty();
        emit changeLevel(level);
        ui->myGLWidget->newTarget();   
        nomJoueur = x.getName();
        countGame = 0;
        ui->labelCibleJ->setText("0");
        ui->labelCibleR->setText("10");
         ui->labelScore->setText("0");
        switch(level){
        case 1: ui->labelLevel->setText("Facile");
        break;
         case 2: ui->labelLevel->setText("Moyen");
        break;
         case 3: ui->labelLevel->setText("Difficile");
        break;
        }
        timeRef = (QTime::currentTime().toString("hh:mm:ss"));
        chronoTotal->start(1000);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space){
        go=true;
        int result_cols =  image.cols - templateImage.cols + 1;
        int result_rows = image.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );
        matchImage=templateImage;
    }else if (event->key() == Qt::Key_P){
        go=true;
        int result_cols =  image.cols - templateImage.cols + 1;
        int result_rows = image.rows - templateImage.rows + 1;
        resultImage.create( result_cols, result_rows, CV_32FC1 );
        matchImage=templateImage;
    }
}

void MainWindow::chronoRefresh()
{
    QStringList list = timeRef.split(":");
    QString heure1 = list.join(QString(":"));
    list.removeFirst();
    heure1=heure1.mid(0,2);
    QString minute1 = list.join(QString(":"));
    minute1=minute1.mid(0,2);
    list.removeFirst();
    QString seconde1 = list.join(QString(":"));
   QString timeNow = (QTime::currentTime().toString("hh:mm:ss"));
   QStringList list1 = timeNow.split(":");
   QString heure2 = list1.join(QString(":"));
   heure2=heure2.mid(0,2);
   list1.removeFirst();
   QString minute2 = list1.join(QString(":"));
   minute2=minute2.mid(0,2);
   list1.removeFirst();
   QString seconde2 = list1.join(QString(":"));
   int newTimeHH =heure2.toInt() -heure1.toInt();
   int newTimeMM = minute2.toInt() - minute1.toInt();
   int newTimeSS = seconde2.toInt() - seconde1.toInt();
   if (newTimeSS<0){
       newTimeSS=newTimeSS+60;
       newTimeMM=newTimeMM-1;
   }
   if (newTimeMM<0){
       newTimeMM=newTimeMM+60;
       newTimeHH=newTimeHH-1;
   }
   if (newTimeHH<0){
       newTimeHH=newTimeHH+24;
   }
    ui->labelChronoTot->setText(QString::number(newTimeHH)+":"+QString::number(newTimeMM)+":"+QString::number(newTimeSS));
    timeFinal=(QString::number(newTimeHH)+":"+QString::number(newTimeMM)+":"+QString::number(newTimeSS));
}

void MainWindow::chronoRefresh2()
{
    QStringList list = timeRefC.split(":");
    QString heure1 = list.join(QString(":"));
    list.removeFirst();
    heure1=heure1.mid(0,2);
    QString minute1 = list.join(QString(":"));
    minute1=minute1.mid(0,2);
    list.removeFirst();
    QString seconde1 = list.join(QString(":"));
   QString timeNow = (QTime::currentTime().toString("hh:mm:ss"));
   QStringList list1 = timeNow.split(":");
   QString heure2 = list1.join(QString(":"));
   heure2=heure2.mid(0,2);
   list1.removeFirst();
   QString minute2 = list1.join(QString(":"));
   minute2=minute2.mid(0,2);
   list1.removeFirst();
   QString seconde2 = list1.join(QString(":"));

   int newTimeHH =heure2.toInt() -heure1.toInt();
   int newTimeMM = minute2.toInt() - minute1.toInt();
   int newTimeSS = seconde2.toInt() - seconde1.toInt();
   if (newTimeSS<0){
       newTimeSS=newTimeSS+60;
       newTimeMM=newTimeMM-1;
   }
   if (newTimeMM<0){
       newTimeMM=newTimeMM+60;
       newTimeHH=newTimeHH-1;
   }
   if (newTimeHH<0){
       newTimeHH=newTimeHH+24;
   }
    ui->labelChronoCible->setText(QString::number(newTimeHH)+":"+QString::number(newTimeMM)+":"+QString::number(newTimeSS));

}
void MainWindow::setScore(int sco){
    if(countGame < 10){
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
        ui->labelScore->setText(QString::number(score));
    }
}

void MainWindow::loadHighScore(){
    QSettings highScore("Catakoi", "highScore");
    //Facile
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(highScore.value("ScoreFacile/Nom", "Valeille").toString()));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem(highScore.value("ScoreFacile/Score", 0).toString()));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem(highScore.value("ScoreFacile/Chrono", "").toString()));
    //Intermediaire
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(highScore.value("ScoreIntermediaire/Nom", "Catakoi").toString()));
    ui->tableWidget->setItem(1,1,new QTableWidgetItem(highScore.value("ScoreIntermediaire/Score", 0).toString()));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem(highScore.value("ScoreIntermediaire/Chrono", "").toString()));
    //Difficile
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(highScore.value("ScoreDifficile/Nom", "Ricaud").toString()));
    ui->tableWidget->setItem(1,2,new QTableWidgetItem(highScore.value("ScoreDifficile/Score", 0).toString()));
    ui->tableWidget->setItem(2,2,new QTableWidgetItem(highScore.value("ScoreDifficile/Chrono", "").toString()));
}

void MainWindow::saveHighScore(){

    QSettings highScore("Catakoi", "highScore");
    highScore.setValue("ScoreFacile/Nom", ui->tableWidget->item(0,0)->text());
    highScore.setValue("ScoreFacile/Score", ui->tableWidget->item(1,0)->text().toInt());
    highScore.setValue("ScoreFacile/Chrono", ui->tableWidget->item(2,0)->text());
    //Intermediaire
    highScore.setValue("ScoreIntermediaire/Nom", ui->tableWidget->item(0,1)->text());
    highScore.setValue("ScoreIntermediaire/Score", ui->tableWidget->item(1,1)->text().toInt());
    highScore.setValue("ScoreIntermediaire/Chrono", ui->tableWidget->item(2,1)->text());
    //Difficile
    highScore.setValue("ScoreDifficile/Nom", ui->tableWidget->item(0,2)->text());
    highScore.setValue("ScoreDifficile/Score", ui->tableWidget->item(1,2)->text().toInt());
    highScore.setValue("ScoreDifficile/Chrono", ui->tableWidget->item(2,2)->text());
}

void MainWindow::clearHighScore(){
    QSettings highScore("Catakoi", "highScore");
    highScore.clear();
}

void MainWindow::displayHighScore(){
    switch(level){
    case 1 :
    {
        int scoreF = ui->tableWidget->item(1,0)->text().toInt();
        if (score>scoreF){
            ui->tableWidget->setItem(0,0,new QTableWidgetItem(nomJoueur));
            ui->tableWidget->setItem(1,0,new QTableWidgetItem(QString::number(score)));
            ui->tableWidget->setItem(2,0,new QTableWidgetItem(timeFinal));
        }else if(score==scoreF){
            QString chronoF = ui->tableWidget->item(2,0)->text();
            if(timeFinal<chronoF){
                ui->tableWidget->setItem(0,0,new QTableWidgetItem(nomJoueur));
                ui->tableWidget->setItem(1,0,new QTableWidgetItem(QString::number(score)));
                ui->tableWidget->setItem(2,0,new QTableWidgetItem(timeFinal));
            }
        }
     break;
    }
    case 2 :
    {
    int scoreI = ui->tableWidget->item(1,1)->text().toInt();
    if (score>scoreI){
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(nomJoueur));
        ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(score)));
        ui->tableWidget->setItem(2,1,new QTableWidgetItem(timeFinal));
    }else if(score==scoreI){
        QString chronoI = ui->tableWidget->item(2,1)->text();
        if(timeFinal<chronoI){
            ui->tableWidget->setItem(0,1,new QTableWidgetItem(nomJoueur));
            ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(score)));
            ui->tableWidget->setItem(2,1,new QTableWidgetItem(timeFinal));
        }
    }
    break;}
    case 3 :
    {
        int scoreD = ui->tableWidget->item(1,2)->text().toInt();
        if (score>scoreD){
            ui->tableWidget->setItem(0,2,new QTableWidgetItem(nomJoueur));
            ui->tableWidget->setItem(1,2,new QTableWidgetItem(QString::number(score)));
            ui->tableWidget->setItem(2,2,new QTableWidgetItem(timeFinal));
        }else if(score==scoreD){
            QString chronoD = ui->tableWidget->item(2,2)->text();
            if(timeFinal<chronoD){
                ui->tableWidget->setItem(0,2,new QTableWidgetItem(nomJoueur));
                ui->tableWidget->setItem(1,2,new QTableWidgetItem(QString::number(score)));
                ui->tableWidget->setItem(2,2,new QTableWidgetItem(timeFinal));
            }
        }
    break;}
    default:
        break;
          }
    }
