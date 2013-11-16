#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->label->setText(QString::fromStdString(myPlayer->name));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    myNewDialog = new Dialog;
//    myNewDialog->show();
//}

void MainWindow::setDisplayData(player *p, QVector<champion*> *c, QVector<match*> *m)
{
    myPlayer = p;
    myChampionList = c;
    myMatchList = m;
}

void MainWindow::setChampData(std::vector<champion> c)
{
    if(c.size() == 0)
        std::cout<<"no champ data given"<<std::endl;
    else
        std::cout<<c[0].write()<<std::endl;
    mcl = QVector<champion>::fromStdVector(c);
}


/*
 *
 *  Main function to display intial data to all tabs
 *
 */
void MainWindow::displayData()
{
    workingName = "TestSummoner";
    //Settings tab data display
    ui->nameEdit->setText(workingName);
    //std::cout << ui->settingsNameLabel->text().toAscii.data();
    //qDebug() << ui->settingsNameLabel->text();


    //**********************
    //Champion data display
    //**********************
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
                    "B vs P Winrate" <<
                    "Creep Score" <<
                    "Neutral Creeps" <<
                    "Enemy Minions" <<
                    "Avg CS" <<
                    "Avg Neutral CS" <<
                    "Avg Minions";
    ui->championsTableWidget->setColumnCount(horizHeaders.count());
    ui->championsTableWidget->setHorizontalHeaderLabels(horizHeaders);
    ui->championsTableWidget->setRowCount(mcl.count());

    //display champion stats. Display calls must be in same order as headers
    int j = 0;
    for (int i=0; i<mcl.count(); i++)
    {
        j = 0;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::fromStdString( mcl[i].getName() ) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getKills()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getDeaths()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getAssists()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].KDA()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getWins()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getLosses()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].winRate(0)) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getBlueWins()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getBlueLosses()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getPurpleWins()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getPurpleLosses()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].winRate(2)) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getCreepScore()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getNeutralCreeps()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].getEnemyMinions()) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(0)) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(1)) ));
        j++;
        ui->championsTableWidget->setItem(i, j, new QTableWidgetItem( QString::number(mcl[i].averages(2)) ));
    }

    ui->championsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //*************************
    //Display match history tab
    //*************************

    //display match list
    ui->matchListWidget->clear();
    for(int i=0; i<mml.count(); i++)
    {
        ui->matchListWidget->addItem(new QListWidgetItem( "match " + QString::number(mml[i].getID()) ));
    }
    ui->matchListWidget->setCurrentRow(0);
}

//confirm a name change in the settings tab
void MainWindow::on_nameSetButton_clicked()
{
    workingName = ui->nameEdit->toPlainText();

    QFont f = ui->nameEdit->font();
    f.setBold(false);
    ui->nameEdit->setFont(f);
}

//cancel a name change in the settings tab
void MainWindow::on_nameCancleButton_clicked()
{
    ui->nameEdit->setText(workingName);
}

//bold and unbold the name change field in the settings tab to indicate a change was made
void MainWindow::on_nameEdit_textChanged()
{
    QFont f = ui->nameEdit->font();

    f.setBold( ui->nameEdit->toPlainText() != workingName );

    ui->nameEdit->setFont(f);

}

//display the match stats for the currently selected row in matchListWidget
void MainWindow::on_matchListWidget_currentRowChanged(int currentRow)
{
    QString s = "match id: "+ QString::number(mml[currentRow].getID()) +"\n" +
    "champ id: " + QString::number(mml[currentRow].getChampID());

    ui->matchStatsLabel->setText(s);
}
