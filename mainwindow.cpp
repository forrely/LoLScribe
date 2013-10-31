#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mp = player();
    mcl = QVector<champion>();

    //ui->label->setText(QString::fromStdString(myPlayer->name));
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

void MainWindow::setDisplayData(player *p, QVector<champion*> *c, QVector<match*> *m)
{
    myPlayer = p;
    myChampionList = c;
    myMatchList = m;
}

void MainWindow::displayData()
{
    mcl[0].name = "test";
    mcl[0].kills = 2;

    //Champion data display
    //horizHeaders indicates display name
    QStringList horizHeaders;
    horizHeaders << "Name" <<
                    "Kills" <<
                    "Deaths" <<
                    "Assists" <<
                    "KDA" <<
                    "Wins" <<
                    "Losses" <<
                    "Winrate" <<
                    "Blue Wins" <<
                    "Blue Losses" <<
                    "Purple Wins" <<
                    "Purple Losses" <<
                    "B vs P Winrate"
                    "Creep Score" <<
                    "Neutral Creeps" <<
                    "Enemy Minions" <<
                    "Avg CS" <<
                    "Avg Neutral CS" <<
                    "Avg Minions";
    ui->tableWidget_Champions->setColumnCount(horizHeaders.count());
    ui->tableWidget_Champions->setHorizontalHeaderLabels(horizHeaders);
    ui->tableWidget_Champions->setRowCount(mcl.count());


    //display champion stats. Display calls should be in same order as headers
    int j = 0;
    for (int i=0; i<mcl.count(); i++)
    {
        j = 0;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::fromStdString( mcl.at(i).name ) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).kills) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).deaths) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).assists) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].KDA()) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).wins) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).losses) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].winRate(0)) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).blueWins) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).blueLosses) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).purpleWins) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).purpleLosses) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].winRate(2)) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).creepScore) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).neutralCreeps) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl.at(i).enemyMinions) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(0)) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(1)) ));
        j++;
        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(2)) ));
//        j = 0;
//        //ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::fromStdString( myChampionList->at(i)->name ) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->kills) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->deaths) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->assists) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->KDA()) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->wins) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->losses) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->winRate(0)) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->blueWins) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->blueLosses) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->purpleWins) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->purpleLosses) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->winRate(2)) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->creepScore) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->neutralCreeps) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->enemyMinions) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->averages(0)) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->averages(1)) ));
//        j++;
//        ui->tableWidget_Champions->setItem(i, j, new QTableWidgetItem( QString::number(myChampionList->at(i)->averages(2)) ));
    }

    ui->tableWidget_Champions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
