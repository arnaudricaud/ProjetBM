#include "sauvegarde.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

using namespace std;


sauvegarde::sauvegarde(QString chemin)
{
chemin_=chemin;
}





void sauvegarde::sauvegardeScore(int score, int difficulte, QString nomJoueur){
    QFile file(chemin_);
    if (file.open(QFile::Append | QFile::Text)) {
        QTextStream myfile(&file);
        myfile <<score<<"|"<<difficulte<<"|"<<nomJoueur<<endl;
    }
    else qDebug() << "Erreur d'ouverture du fichier";
}

QString sauvegarde::getScore(int difficulte){
    int max=0;
    QString final="";
    QStringList temp;
    QFile inputFile(chemin_);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            temp=line.split("|");
            if (temp[1].toInt()==difficulte){
                if (temp[0].toInt()>max){
                    final=("Joueur :"+temp[2]+", score : "+temp[0]);
                    max=temp[0].toInt();
                }
            }
        }
        inputFile.close();
    }
    return final;
}
