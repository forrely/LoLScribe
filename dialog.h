#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    //void setData(MainWindow m);
    void updateGraph();

public:
    //MainWindow myMainWindow;


private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
