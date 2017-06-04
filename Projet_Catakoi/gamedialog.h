#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include<QString>
#include <QDialog>

namespace Ui {
class gameDialog;
}

class gameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit gameDialog(QWidget *parent = 0);
    ~gameDialog();
    QString getName();
    int getDifficulty();

private slots:

    void on_lineEditNom_textChanged(const QString &arg1);

    void on_radioButtonF_clicked();

    void on_radioButtonI_clicked();

    void on_radioButtonD_clicked();

private:
    Ui::gameDialog *ui;
    int level;
    QString name;
};

#endif // GAMEDIALOG_H
