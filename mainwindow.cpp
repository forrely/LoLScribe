#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPlayer = new player("blah", 0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    myChampionList = new QVector<champion>();
    myMatchList = new QVector<match>();

    ui->label->setText(QString::fromStdString(myPlayer->name));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    myNewDialog = new Dialog;
    myNewDialog->show();
}
