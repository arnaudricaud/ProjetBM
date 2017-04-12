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

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int scale);
    void setAngleCatapulte(int angle);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zoomChanged(int scale);
    void angleCatapulteChanged(int angle);

private:
    void draw();
    void drawCube();
    void drawTrebuchet();
    void drawPied();
    void drawContrepoid();
    void drawBras();
    void drawSol();
    void drawCiel();
    int angleCatapulte;
    int angleBras;
    int xRot;
    int yRot;
    int zRot;
    float zoom;

    QPoint lastPos;
};

#endif // MYGLWIDGET_H
