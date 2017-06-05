#include "gamedialog.h"
#include "ui_gamedialog.h"
#include <QDebug>

gameDialog::gameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameDialog)
{
    ui->setupUi(this);
}

gameDialog::~gameDialog()
{
    delete ui;
}

QString gameDialog::getName()
{
    return name;
}


int gameDialog::getDifficulty()
{
    return level;
}


void gameDialog::on_radioButtonF_clicked()
{
    level=1;
}

void gameDialog::on_radioButtonI_clicked()
{
    level=2;
}

void gameDialog::on_radioButtonD_clicked()
{
    level=3;
}
