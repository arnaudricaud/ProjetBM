#include "score.h"

Score::Score()
{

}

Score::Score(int dL)
{
    name="undefined";
    difficultyLevel=dL;
    points=0;
}

Score::Score(QString nom, int dL, int pts)
{
    name=nom;
    difficultyLevel=dL;
    points=pts;
}

QString Score::getName()
{
    return name;
}


int Score::getDifficultyLevel()
{
    return difficultyLevel;
}

int Score::getPoints()
{
    return points;
}

void Score::setName(string nom)
{
    name_=QString::fromStdString(nom);



void Score::setDifficultyLevel(int dL)
{
    this->difficultyLevel=dL;
}

void Score::setPoints(int pts)
{
    this->points=pts;
}
