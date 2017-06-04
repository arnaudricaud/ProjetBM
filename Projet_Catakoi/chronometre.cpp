#include "chronometre.h"

Chronometre::Chronometre()
{

        timer1 = new QTimer();
        timer1->setInterval(1000);
        connect(timer1, SIGNAL(timeout()), this, SLOT(incrementerTemps())); // le timer n'est pas encore démarré mais rien n'empêche de déjà le connecter
    }

    void Chronometre::demarrer()
    {
        m_tempsActuel = 0; // pour redémarrer s'il l'est déjà on remet à 0, si on ne veut pas qu'il redémarre on met cette ligne dans le constructeur
        timer1->start();
    }

    void Chronometre::incrementerTemps()
    {
        m_tempsActuel++; // on ajoute 1 au temps actuel
       // setText("Temps écoulé : " + QString::number(m_tempsActuel)); // on actualise le temps affiché
    }
