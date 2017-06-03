#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include<QString>

class sauvegarde
{
public:
    sauvegarde(QString chemin);
    void sauvegardeScore(int score, int difficulte, QString nomJoueur);
    QString getScore(int difficulte);

private:
QString chemin_;

};


#endif // SAUVEGARDE_H
