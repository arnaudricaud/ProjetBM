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
    void drawWoodCube();
    void drawTrebuchet();
    void drawPied();
    void drawContrepoid();
    void drawBras();
    void drawStadium();
    void drawPancarte();
    void drawTarget();
    void drawSol();
    void drawCiel();
    void drawFilet();
    void drawLogo();
    void drawBall();
    void drawImpact();


    void showImpact();
    void calcBall();    //Calcul de la trajectoire de la balle, et des informations de l'impact
    void newTarget(); //Génération d'une cible en fonction de la difficulté

    int level; //Niveau de difficulté (de 1 à 3 pour Facile; Moyen; Difficile)
    bool visibleImpact;
    float impactDist; //Distance entre l'impact et le centre de la cible (on peut en déduire le score)
    float impactX;
    float impactY;

    bool ballThrow;
    int angleCatapulte; // Angle de la catapulte par rapport au sol
    int angleBras;  // Angle du bras de la catapulte
    float angleCorde;
    int puissance;  //Puissance de lancement

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
    //TEXTURES:
    QImage logoTSE;
    QImage pancarte;

    QPoint lastPos;
};

#endif // MYGLWIDGET_H
