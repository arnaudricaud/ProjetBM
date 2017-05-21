// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>
#include <QGLWidget>
#include <QOpenGLTexture>
#include <GL/glu.h>
#include <time.h>
#include "myglwidget.h"



MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    //TEXTURES:

    //CAMERA:
    setFocusPolicy(Qt::StrongFocus);
    xTra = 0;
    yTra = -0.6;
    zTra = -10;

    xRot = -85*16;
    yRot = 0;
    zRot = -90*16;
    zoom = 0.012;


    puissance = 100;
    angleCatapulte = 0;
    angleBras = 40;

    srand (time(NULL));
    xTarget = -( rand() % 350 + 250);
    yTarget = rand() % 400 - 200;

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
        angle += 360*16;
    while (angle > (360*16))
        angle -= 360*16;
}

void MyGLWidget::setXRotation(int angle)
{

    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
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
    if (angle > 38){
        angle = 38;
    } else if (angle < -135){
        angle = -135;
    }
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
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    //static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    //static GLfloat light2Position[4] = {0, 10, 0, 1.0};
    //static GLfloat light2Direction[4] = {0, 0, 0, 1.0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //glLightfv(GL_LIGHT1, GL_POSITION, light2Position);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light2Direction);


    //Chargement des textures:

}

void MyGLWidget::paintGL()
{

    resizeGL(this->width(), this->height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xTra, yTra, zTra);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(zoom,zoom,zoom);
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
    glOrthof(-widht2, widht2, -height2, height2, 1.0, 40.0);
#else
    glOrtho(-widht2, widht2, -height2, height2, 1.0, 40.0);
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
    drawBall();
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
            //Dessin corde
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

void MyGLWidget::drawTarget(){
    glPushMatrix();
        glColor3f(1, 0.3, 0.3);
        glTranslatef(xTarget, yTarget, 25);
        glRotatef(60,0,1,0);
        glPushMatrix();
            glScalef(10,10,2);
            drawCube();
        glPopMatrix();

        glColor3f(1, 1, 1);
        glPushMatrix();
            glTranslatef(0,0,-0.1);
            glScalef(20,20,2);
            drawCube();
        glPopMatrix();

        glColor3f(1, 1, 0.2);
        glPushMatrix();
            glTranslatef(0,0,-0.2);
            glScalef(30,30,2);
            drawCube();
        glPopMatrix();

        glColor3f(1, 1, 1);
        glPushMatrix();
            glTranslatef(0,0,-0.3);
            glScalef(50,50,2);
            drawCube();
        glPopMatrix();
    glPopMatrix();

}

void MyGLWidget::launchBall(){
    qDebug()<<"BALLE LANCEE";
    /*
    //INITIALISATION DE LA CAMERA (OK)
    xTra = 0;
    yTra = -0.6;
    zTra = -10;
    xRot = -70*16;
    yRot = 0;
    zRot = -140*16;
    zoom = 0.012;
    */

    //Pour tests
    xTra = -1.5;
    yTra = -0.6;
    zTra = -10;
    xRot = -70*16;
    yRot = 0;
    zRot = -180*16;
    zoom = 0.005;

    //DESSINS AVEC LA BALLE (PAS DE PRISE EN COMPTE DE LA PUISSANCE)
    for(int i = 40; i > -130; i--){
        setAngleBras(i);
    }
    for(int i = -130; i < -70; i++){
        setAngleBras(i);
    }
    for(int i = -70; i > -90; i--){
        setAngleBras(i);
        Sleep(10);
    }

    calcBall();


}
void MyGLWidget::drawBall(){
    glPushMatrix();
        glRotatef(angleCatapulte,0,0,1);
        glTranslatef(30,-5,3);
        glTranslatef(ballPosition[0], ballPosition[1], ballPosition[2]);
        glScalef(4,4,4);
        drawCube();
    glPopMatrix();
}


void MyGLWidget::calcBall(){
    //1m = 2*coord
    //1 Tick => 10ms!!
    // Gravité => 10m/s => 0.2coord/tick
    ballSpeed[0] = float(puissance)/10;
    ballSpeed[1] =  0;
    ballSpeed[2] = float(puissance)/15;
    ballPosition[0] = 0;
    ballPosition[1] = 0;
    ballPosition[2] = 50;

    while (ballSpeed[0] != 0 || ballSpeed[1] != 0 || ballSpeed[2] != 0){

        ballPosition[0] -= ballSpeed[0];
        ballPosition[1] += ballSpeed[1];
        ballPosition[2] += ballSpeed[2];

        if (ballPosition[0] < -740){
            ballPosition[0] = -740;
            ballSpeed[0] = 0;
        }

        if (ballPosition[2] > 0){
            ballSpeed[2] -= 0.2;
        } else{
            ballSpeed[2] = 0;
            ballPosition[2] = 0;
            ballSpeed[0] -= 0.3;
            if (ballSpeed[0] < 0){
                ballSpeed[0] = 0;
            }
        }
       updateGL();
    Sleep(10);
    }
}
