// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QOpenGLTexture>
#include <GL/glu.h>
#include "myglwidget.h"

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXTranslation(float dist);
    void setYTranslation(float dist);
    void setZTranslation(float dist);

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int scale);
    void setAngleCatapulte(int angle);
    void setAngleBras(int angle);
    void launchBall();

signals:
    // signaling rotation from mouse movement
    void xTranslationChanged(float dist);
    void yTranslationChanged(float dist);
    void zTranslationChanged(float dist);

    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zoomChanged(int scale);
    void angleCatapulteChanged(int angle);
    void angleBrasChanged(int angle);

private:
    void draw();
    void drawCube();
    void drawTrebuchet();
    void drawPied();
    void drawContrepoid();
    void drawBras();
    void drawStadium();
    void drawTarget();
    void drawSol();
    void drawCiel();
    void drawFilet();
    void drawBall();
    void calcBall();
    void newTarget();

    int score;

    int angleCatapulte;
    int angleBras;
    int puissance;
    int xRot;
    int yRot;
    int zRot;

    float xTra;
    float yTra;
    float zTra;

    float xTarget;
    float yTarget;

    float ballSpeed [3] = {0, 0, 0};
    float ballPosition [3] = {0, 0, 0};



    float zoom;

    QPoint lastPos;
};

#endif // MYGLWIDGET_H
