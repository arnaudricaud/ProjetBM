// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include <QGLWidget>
#include <QOpenGLTexture>
#include <GL/glu.h>
#include <time.h>
#include <math.h>
#include "myglwidget.h"



MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //TEXTURES:

    //CAMERA:
    setFocusPolicy(Qt::StrongFocus);
    xTra = 0;
    yTra = 0;
    zTra = -15;

    xRot = -80*16;
    yRot = 0;
    zRot = -90*16;
    zoom = 0.012;




    puissance = 62;
    angleCatapulte = 0;
    setAngleBras(30);
    angleCorde = 145;
    ballThrow = false;

    visibleImpact = false;
    impactX = 0;
    impactY = 0;


    level = 2;
    newTarget();

}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle = 360*16;
    while (angle > (360*16))
        angle -= 360*16;
}

void MyGLWidget::setXRotation(int angle)
{

    while (angle < -90*16)
        angle = -90*16;
    while (angle > (0*16))
        angle = 0*16;

    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    while (angle < -90*16)
        angle = -90*16;
    while (angle > (0*16))
        angle = 0*16;

    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setXTranslation(float dist)
{
    xTra = xTra + dist;

    emit xTranslationChanged(xTra);
    updateGL();
}

void MyGLWidget::setYTranslation(float dist)
{
    yTra = yTra + dist;
    emit yTranslationChanged(yTra);
    updateGL();
}

void MyGLWidget::setZTranslation(float dist)
{
    zTra = zTra + dist;
    emit zTranslationChanged(zTra);
    updateGL();
}


void MyGLWidget::setZoom(int scale)
{
    zoom = float(scale)/1000;
    emit zoomChanged(scale);
    updateGL();
}

void MyGLWidget::setAngleCatapulte(int angle)
{
    angleCatapulte = angle;
    emit angleCatapulteChanged(angle);
        updateGL();
}

void MyGLWidget::setAngleBras(int angle)
{
    if (angle > 30){
        angle = 30;
    } else if (angle < -135){
        angle = -135;
    }
    angleCorde = (115 + angleBras*1.3);
    angleBras = angle;
    emit angleBrasChanged(angle);
        updateGL();
}


void MyGLWidget::initializeGL()
{
    qglClearColor(Qt::black);



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
//LUMIERES
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0); //SOLEIL
//    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
//    float specLight0[4] = {0.5f, 0.5f, 0.5f, 1.0f};
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, specLight0);


    //Chargement des textures:

}

void MyGLWidget::paintGL()
{

    resizeGL(this->width(), this->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(zoom,zoom,zoom);
    glTranslatef(xTra, yTra, zTra);

    draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    float widht2 = 2*float(width)/600;
    float height2 = 2*float(height)/600;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-widht2, widht2, -height2, height2, -50.0, 40.0);
#else
    glOrtho(-widht2, widht2, -height2, height2, -50.0, 40.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + dy*8);
        setYRotation(yRot + dx*8);
    } else if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + dy*8);
        setZRotation(zRot + dx*8);
    }

    lastPos = event->pos();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Z){
        float zoomInt = zoom*1000;
        int scale = (int)zoomInt + 2;
        setZoom(scale);
    } else if(event->key() == Qt::Key_S){
        float zoomInt = zoom*1000;
        int scale = (int)zoomInt - 2;
        setZoom(scale);
    } else if(event->key() == Qt::Key_Q){
        setXTranslation(0.3);
    }else if(event->key() == Qt::Key_D){
        setXTranslation(-0.3);
    }else if(event->key() == Qt::Key_A){
        setYTranslation(0.3);
    }else if(event->key() == Qt::Key_E){
        setYTranslation(-0.3);
    }
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    int scroll = (event->angleDelta().y())/60;
    float zoomInt = zoom*1000;
    int scale = (int)zoomInt + scroll;
    setZoom(scale);
}



void MyGLWidget::draw()
{

    glColor3f(1,1,1);
    glPushMatrix();
        glScalef(2,2,2);
        drawTrebuchet();
    glPopMatrix();
    drawStadium();
    drawTarget();
    glColor3f(0.5, 0.5, 0.5);
    drawBall();
    drawImpact();
    //drawCiel();
}

void MyGLWidget::drawCube()
{

    glBegin(GL_QUADS);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3d(0.5f, -0.5f, -0.5f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3d(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3d(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3d(0.5f, 0.5f, -0.5f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3d(-0.5f, 0.5f, -0.5f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3d(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

}

void MyGLWidget::drawTrebuchet()
{
    glPushMatrix();
        glRotatef(angleCatapulte,0,0,1);
        glTranslatef(0,-6,0);
        GLUquadric* cylindre = gluNewQuadric();
        glPushMatrix();
            drawPied();
            glTranslatef(0,10,0);
            drawPied();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(7,11,14);
            glPushMatrix();
                glRotatef(90,1,0,0);
                glScalef(1,1,12);
                gluCylinder(cylindre, 0.5,0.5,1,32,32);
            glPopMatrix();
            glTranslatef(0,-6,0);
            drawBras();
        glPopMatrix();
    glPopMatrix();
}

void MyGLWidget::drawPied()
{
    glPushMatrix();
        glColor3f(0.53, 0.45, 0.34);
        glPushMatrix();
            glTranslatef(15,0,0);
            glScalef (30, 1, 1);
            drawCube();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(4, -0.25, 7);
            glPushMatrix();
                glRotatef(-65,0,1,0);
                glScalef (15, 0.5, 1);
                drawCube();
            glPopMatrix();
            glPushMatrix();
                glTranslatef(6, 0.5, 0);
                glRotatef(65,0,1,0);
                glScalef (15, 0.5, 1);
                drawCube();
            glPopMatrix();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(7,0,13.5);
            glScalef(4,1.5,1);
            drawCube();
        glPopMatrix();
    glPopMatrix();
}


void MyGLWidget::drawContrepoid()
{
    glColor3f(0.43, 0.35, 0.24);
        glPushMatrix();
            glTranslatef(0,0,-2);
            glScalef (0.1, 0.1, 4);
            drawCube();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,0,-6);
            glScalef (5,5,4);
            drawCube();
       glPopMatrix();
}


void MyGLWidget::drawBras()
{
        glRotatef(angleBras,0,1,0);
        glPushMatrix();
        glTranslatef(7.5,0,0);
        glPushMatrix();
            glScalef(30,1.5,1.5);
            drawCube();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-12,0,0);
            glRotatef(angleBras,0,-1,0);
            drawContrepoid();
        glPopMatrix();
        //Corde
        glPushMatrix();
            glTranslatef(15,0,0);
            glRotatef(angleCorde,0,1,0);
            glTranslatef(6,0,0);
            glPushMatrix();
                glScalef(12,0.5,0.5);
                drawCube();
            glPopMatrix();
            //Ball
            if(!ballThrow){
                glPushMatrix();
                GLUquadric* sphere = gluNewQuadric();
                glColor3f(0.7,0.7,0.7);
                glTranslatef(6,0,0);
                glScalef(2,2,2);
                gluSphere(sphere, 1, 32,32);
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
}

void MyGLWidget::drawStadium(){
    drawSol();
    //Filets:
    glPushMatrix();
        glTranslatef(30,162,0); // 150 + 16 (3*filet/2 + moitier de catapulte)
        glRotatef(135,0,0,1);
        drawFilet();
        glTranslatef(200,0,0);
        glRotatef(45,0,0,1);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();


        glTranslatef(200,0,0);
        glRotatef(90,0,0,1);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();

        glTranslatef(200,0,0);
        glRotatef(90,0,0,1);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();
        glTranslatef(200,0,0);
        drawFilet();

        glTranslatef(200,0,0);
        glRotatef(45,0,0,1);
        drawFilet();
        glTranslatef(200,0,0);
        //Dernier Poteau
        glPushMatrix();
            glColor3f(0.5, 0.5, 0.5);
            glTranslatef(0,0,35);
            glScalef(2,2,70);
            drawCube();
        glPopMatrix();

    glPopMatrix();
}

void MyGLWidget::drawSol()
{
    glPushMatrix();
        glTranslatef(-300,0,-0.5);
        glColor3f(0.14, 0.35, 0.0);
        glScalef(1000,700,1);
        drawCube();
    glPopMatrix();


}
void MyGLWidget::drawCiel()
{

}
void MyGLWidget::drawFilet()
{
    glPushMatrix();
        //Dessin du Premier Poteau
        glPushMatrix();
            glColor3f(0.5, 0.5, 0.5);
            glTranslatef(0,0,35);
            glScalef(2,2,70);
            drawCube();
        glPopMatrix();
        glPushMatrix();
            //Dessin filets
            glColor3f(0.7, 0.54, 0.41);
            glTranslatef(100,0,0);
            glPushMatrix();
                for(int i = 0; i < 20; i++){
                    glTranslatef(0,0,3);
                    glPushMatrix();
                        glScalef(200,0.3,0.3);
                        drawCube();
                    glPopMatrix();
                }
            glPopMatrix();
        glPopMatrix();
        //Cordes Verticales
        glTranslatef(0,0,30);
        glPushMatrix();
            for(int i = 0; i < 50; i++){
                glTranslatef(4,0,0);
                glPushMatrix();
                    glScalef(0.3,0.3,60);
                    drawCube();
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
}

void MyGLWidget::newTarget(){
    srand (time(NULL));

    switch (level){
    case 1:
        xTarget = -( rand() % 100 + 250);
        break;
    case 2:
        xTarget = -( rand() % 100 + 350);
        break;
    case 3:
        xTarget = -( rand() % 160 + 450);
        break;
    }
    yTarget = rand() % 400 - 200;

    updateGL();
}

void MyGLWidget::drawTarget(){
    GLUquadric* disk = gluNewQuadric();
    glPushMatrix();
        glColor3f(1, 1, 0.3);
        glTranslatef(xTarget, yTarget, 0.1);
        glScalef(50,50,1);
        gluDisk(disk, 0,1, 32,1);
    glPopMatrix();

}


void MyGLWidget::drawImpact(){
    if (visibleImpact)
    {
        glColor3f(1,0,0.5);
        glPushMatrix();
            glTranslatef(impactX,impactY,1);
            glScalef(4,4,1);
            drawCube();
        glPopMatrix();
    }
}

void MyGLWidget::showImpact(){
    //CAM AU DESSUS DE L'IMPACT
    xTra = -xTarget;
    yTra = -yTarget;
    zTra = -1;
    xRot = 0;
    yRot = 0;
    zRot = -90*16;
    zoom = 0.02;
    visibleImpact = true; // On montre l'impact
    updateGL();
    Sleep(4000);
    visibleImpact = false; // On retire l'impact
    // RAZ DE LA CAM
    xTra = 0;
    yTra = 0;
    zTra = -5;

    xRot = -80*16;
    yRot = 0;
    zRot = -90*16;
    zoom = 0.012;


}

void MyGLWidget::drawBall(){
    GLUquadric* sphere = gluNewQuadric();
    glColor3f(0.7,0.7,0.7);
    if(ballThrow){
        glPushMatrix();
            float PI = 3.14159265;
            float rad = angleCatapulte*PI/180;
            glTranslatef(ballPosition[0]* cos(rad) - ballPosition[1] * sin(rad),
                    ballPosition[0]* sin(rad) + ballPosition[1] * cos(rad), ballPosition[2]+4);
            glScalef(4,4,4);
            gluSphere(sphere, 1, 32,32);
        glPopMatrix();
    }
}

void MyGLWidget::launchBall(){
    qDebug()<<"BALLE LANCEE";

    //Pour tests

    xTra = 1.5;
    yTra = 0;
    zTra = 0;

    xRot = -70*16;
    yRot = 0;
    zRot = -180*16;
    zoom = 0.008;

    //DESSINS AVEC LA BALLE (PAS DE PRISE EN COMPTE DE LA PUISSANCE)
    for(int i = 30; i > -90; i--){
        setAngleBras(i);
    }
    calcBall();
}


void MyGLWidget::calcBall(){


    //1m = 2*coord
    //1 Tick => 10ms!!
    // Gravité => 10m/s => 0.2coord/tick
    ballSpeed[0] = (float(puissance)+20)/12;
    ballSpeed[1] =  0;
    ballSpeed[2] = (float(puissance)+20)/15;
    ballPosition[0] = 17;
    ballPosition[1] = -2;
    ballPosition[2] = 97;
    float PI = 3.14159265;
    float rad = angleCatapulte * PI/180;
    int posX;
    int posY;


    bool firstTouch = true;
    int frame0 = -90;
    int frame1 = -130;
    int frame2 = -70;
    int frame3 = -100;
    ballThrow = true;

    //CAMERA
        xRot = -70*16;
        zRot = -110*16;
        zoom = 0.01;

    while (ballSpeed[0] != 0 || ballSpeed[1] != 0 || ballSpeed[2] != 0 || frame3 !=-90){

        //SUIVIT DE LA BALLE
        xTra = (-ballPosition[0]);
        yTra = (-ballPosition[1]);
        zTra = (-ballPosition[2]);

        //BRAS -130 => -70 => -90
        if (frame0 != -130){
            angleBras = frame0--;
            angleCorde--;
        }else if (frame1 != -70){
            angleBras = frame1++;
            angleCorde -= 2;
        } else if (frame2 != -100){
            angleBras = frame2--;
            angleCorde--;
        } else if (frame3 != -90){
            angleBras = frame3++;
            angleCorde++;
        }


        //Calcul de la position de la balle
        posX = (ballPosition[0]* cos(rad) - ballPosition[1] * sin(rad)) + 40;
        posY = (ballPosition[0]* sin(rad) + ballPosition[1] * cos(rad)) - 2;

        if(ballPosition[2] == 0 && firstTouch){
            //Calcul de la distance ball/cible
            impactX = posX;
            impactY = posY;
            impactDist = sqrt(pow((posX - xTarget),2) + pow((posY - yTarget),2));
            firstTouch = false;
        }

        ballPosition[0] -= ballSpeed[0];
        ballPosition[1] += ballSpeed[1];
        ballPosition[2] += ballSpeed[2];

        if (posX < -740){
            //ballPosition[0] = -740;
            ballSpeed[0] = 0;
        } else if (posY < -285 || posY > 292){
            ballSpeed[0] = 0;
        }

        if (ballPosition[2] > 0){
            ballSpeed[2] -= 0.2;
        } else{
            ballSpeed[2] = 0;
            ballPosition[2] = 0;
            ballSpeed[0] -= 0.2;
            if (ballSpeed[0] < 0){
                ballSpeed[0] = 0;
            }
        }
       updateGL();
    Sleep(30);
    }
    showImpact();
    ballPosition[0] = 0;
    ballPosition[1] = 0;
    ballPosition[2] = 0;
    setAngleBras(30);
    angleCatapulte = 0;
    newTarget();
    ballThrow = false;
    updateGL();
}
