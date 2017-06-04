#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H
#include <QTimer>

class Chronometre
{
public:
    Chronometre();

public slots:
    void demarrer();
    void incrementerTemps();

private:
    int m_tempsActuel;
    QTimer *timer1 = new QTimer();

};

#endif // CHRONOMETRE_H
