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

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *); // Clavier
    void wheelEvent(QWheelEvent *event); //Molette de la souris

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
    void setPuissance(int puissance);
    void setLevel(int level);
    void launchBall(); // Lancement de la balle

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
    void changeScore(int);

private:
    //DESSIN:
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

    bool ballThrow; // Indique si la balle est en l'air ou non
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

    float xTarget; // Position du centre de la cible
    float yTarget;

    float ballSpeed [3] = {0, 0, 0};    // V de la balle
    float ballPosition [3] = {0, 0, 0}; // Position de la balle


    float zoom;
    //TEXTURES:
    QImage logoTSE;
    QImage pancarte;
    QImage wood;
    QImage rock;

    //Dernière position du curseur (pour les mouvements à la souris)
    QPoint lastPos;
};

#endif // MYGLWIDGET_H
