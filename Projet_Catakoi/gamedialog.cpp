#include "gamedialog.h"
#include "ui_gamedialog.h"

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
    if (level!=1 ||level!=2 || level!=3){
        level =1;
    }
    return level;
}



void gameDialog::on_lineEditNom_textChanged(const QString &arg1)
{
    name=ui->lineEditNom->text();
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
