#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //setup tray icon
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon("Resources/draven icon.png"));
    trayIcon->show();

    QAction* closeAction = new QAction("Close", this);
    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(closeAction);
    trayIcon->setContextMenu(trayIconMenu);

    QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(trayIconCloseAction_triggered()));



    //ui->label->setText(QString::fromStdString(myPlayer->name));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::trayIconCloseAction_triggered() {
    std::cout<<"tray icon stuff"<<std::endl;
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
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->widget->xAxis->setRange(-1, 1);
    ui->widget->yAxis->setRange(0, 1);


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

    //display match details
    displayMatchDetails(ui->matchListWidget->currentRow());
}


void MainWindow::displayMatchDetails(int index)
{
    QVector<QPair<QString, QString>> matchStats;
    matchStats.push_back(QPair<QString, QString>( "Match ID", QString::number(mml[index].getID()) ));
    matchStats.push_back(QPair<QString, QString>( "Champ ID", QString::number(mml[index].getChampID()) ));
    matchStats.push_back(QPair<QString, QString>( "Kills", QString::number(mml[index].getKills()) ));
    matchStats.push_back(QPair<QString, QString>( "Deaths", QString::number(mml[index].getDeaths()) ));
    matchStats.push_back(QPair<QString, QString>( "Assists", QString::number(mml[index].getAssists()) ));
    matchStats.push_back(QPair<QString, QString>( "Damage Dealt", QString::number(mml[index].getDamage()) ));
    matchStats.push_back(QPair<QString, QString>( "Gold", QString::number(mml[index].getGold()) ));


    int numRows = 5;

    //if there aren't the correct number of widgets in grid yet
    if(ui->gridLayout_2->count() != matchStats.count())
    {
        //clear the grid
        QLayoutItem *item;
           while((item = ui->gridLayout_2->takeAt(0))) {
               if (item->widget()) {
                   delete item->widget();
               }
               delete item;
           }

       //populate grid with labels
       QLabel * l;
       for(int i=0, j=0; i<matchStats.size(); i++)
       {
           if(i%5 == 0)
               j++;
           l = new QLabel(matchStats[i].first + ": " + matchStats[i].second);
           l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
           l->setTextInteractionFlags(Qt::TextSelectableByMouse);
           ui->gridLayout_2->addWidget(l, i%5, j);
       }
       ui->gridLayout_2->setRowStretch(numRows, 1);
    }
    else
        for(int i=0, j=0; i<matchStats.size(); i++)
        {
            QLabel* l = qobject_cast<QLabel *> (ui->gridLayout_2->itemAt(i)->widget());
            l->setText(matchStats[i].first + ": " + matchStats[i].second);
        }

}


//**************
//Slots
//**************

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
    displayMatchDetails(currentRow);
    //QString s = "match id: "+ QString::number(mml[currentRow].getID()) +"\n" +
    //"champ id: " + QString::number(mml[currentRow].getChampID());

    //ui->matchStatsLabel->setText(s);
}
