#include "mainwindow.h"
#include "ui_mainwindow.h"

class MyTableWidgetItem : public QTableWidgetItem {


    public:
    MyTableWidgetItem(QString s) : QTableWidgetItem(s)
    {

    }

        bool operator <(QTableWidgetItem *other)
        {
            return text().toFloat() < other->text().toFloat();
        }
};

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

    ui->graphWindowButton->hide();



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

void MainWindow::setDisplayData(player p, QVector<champion> c, QVector<match> m)
{
    mp = p;
    mcl = c;
    mml = m;
}

void MainWindow::setChampTags(QMap<QString, QVector<QString>> tags)
{
    myChampTags = tags;
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
    std::cout<<"mp name in displayData(): "<<mpl[0].getName()<<std::endl;
//    QVector<double> x(101), y(101); // initialize with entries 0..100
//    for (int i=0; i<101; ++i)
//    {
//      x[i] = i/50.0 - 1; // x goes from -1 to 1
//      y[i] = x[i]*x[i];  // let's plot a quadratic function
//    }
//    // create graph and assign data to it:
//    ui->widget->addGraph();
//    ui->widget->graph(0)->setData(x, y);
//    // give the axes some labels:
//    ui->widget->xAxis->setLabel("x");
//    ui->widget->yAxis->setLabel("y");
//    // set axes ranges, so we see all data:
//    ui->widget->xAxis->setRange(-1, 1);
//    ui->widget->yAxis->setRange(0, 1);


    workingName = "TestSummoner";
    //Settings tab data display
    ui->nameEdit->setText(workingName);
    //std::cout << ui->settingsNameLabel->text().toAscii.data();
    //qDebug() << ui->settingsNameLabel->text();




    //**********************
    //Champion data tab display
    //**********************


    setupChampionDetails();
    displayChampionDetails();



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

    // *****************************
        //Display player data
        //**********************************
//        ui->playerListWidget->clear();
//        for(int i=0;i<count();i++)
//        {
//            ui->playerListWidget->addItem(new QListWidgetItem("player" + QString::fromStdString(mp[i].getName())));
//            ui->label1->clear();
//            ui->label1->setText(QString::fromStdString(mp[0].getName()));
//        }
        //ui->playerListWidget->setCurrentRow(0);

        displayPlayerDetails(0);
}

void MainWindow::displayPlayerDetails(int index)
{
    //std::ofstream myfile;
    //myfile.open ("C:\\Users\\dara\\Documents\\GitHub\\LoLScribe\\settingfile.txt");
    //myfile<<"sameer";
    //ui->listWidget_favoriteChampions->clear();
    //ui->listWidget_favoriteChampions->addItems(mp.getFavchamplist());
    std::cout<<"displayed player name: "<<mpl[index].getName()<<std::endl;
     std::cout<<"displayed player kills: "<<mpl[index].getKills()<<std::endl;
      //std::cout<<"displayed player name: "<<mpl[index].getName()<<std::endl;


    QVector<QPair<QString,QString>> playerStats;
    playerStats.push_back(QPair<QString, QString>("Kills", QString::number(mpl[index].getKills())));
    playerStats.push_back(QPair<QString, QString>("Deaths",QString::number(mpl[index].getDeaths())));
    playerStats.push_back(QPair<QString, QString>("Assists",QString::number(mpl[index].getAssists())));
    playerStats.push_back(QPair<QString, QString>("CreepScore",QString::number(mpl[index].getCreepScore())));
    playerStats.push_back(QPair<QString, QString>("NeutralCreeps",QString::number(mpl[index].getNeutralCreeps())));
    playerStats.push_back(QPair<QString, QString>("EnemyMinions",QString::number(mpl[index].getEnemyMinions())));
    playerStats.push_back(QPair<QString, QString>("Wins",QString::number(mpl[index].getWins())));
    playerStats.push_back(QPair<QString, QString>("Losses",QString::number(mpl[index].getLosses())));
    playerStats.push_back(QPair<QString, QString>("BlueWins",QString::number(mpl[index].getBlueWins())));
    playerStats.push_back(QPair<QString, QString>("BlueLosses",QString::number(mpl[index].getBlueLosses())));
    playerStats.push_back(QPair<QString, QString>("PurpleWins",QString::number(mpl[index].getPurpleWins())));
    playerStats.push_back(QPair<QString, QString>("PurpleLosses",QString::number(mpl[index].getPurpleLosses())));
    playerStats.push_back(QPair<QString, QString>("RankedWins",QString::number(mpl[index].getRankedWins())));
    playerStats.push_back(QPair<QString, QString>("RankedLosses",QString::number(mpl[index].getRankedLosses())));

    playerStats.push_back(QPair<QString, QString>("Avg Kills",QString::number(mpl[index].averages(3))));
    playerStats.push_back(QPair<QString, QString>("Avg Deaths",QString::number(mpl[index].averages(4))));
    playerStats.push_back(QPair<QString, QString>("Avg Assists",QString::number(mpl[index].averages(5))));
    playerStats.push_back(QPair<QString, QString>("KDA",QString::number(mpl[index].KDA())));
    playerStats.push_back(QPair<QString, QString>("Avg CS",QString::number(mpl[index].averages(0))));
    playerStats.push_back(QPair<QString, QString>("Avg N CS",QString::number(mpl[index].averages(1))));
    playerStats.push_back(QPair<QString, QString>("Avg E CS",QString::number(mpl[index].averages(2))));
    playerStats.push_back(QPair<QString, QString>("Normal Winrate",QString::number(mpl[index].winRate(0))));
    playerStats.push_back(QPair<QString, QString>("BvP Winrate",QString::number(mpl[index].winRate(2))));
    playerStats.push_back(QPair<QString, QString>("Ranked Winrate",QString::number(mpl[index].winRate(1))));
     ui->playerNameLabel->setText(QString::fromStdString(mpl[index].getName()));  //displaying player name in label
    int numRows = 7;

    //if there aren't the correct number of widgets in grid yet
    if(ui->playerDetailsGrid->count() != playerStats.count())
    {
        //clear the grid
        QLayoutItem *item;
           while((item = ui->playerDetailsGrid->takeAt(0))) {
               if (item->widget()) {
                   delete item->widget();
               }
               delete item;
           }

       //populate grid with labels
       QLabel * l;
       for(int i=0, j=0; i<playerStats.size(); i++)
       {
           if(i%7 == 0)
               j++;
           l = new QLabel(playerStats[i].first + ": " + playerStats[i].second);
           l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
           l->setTextInteractionFlags(Qt::TextSelectableByMouse);
           ui->playerDetailsGrid->addWidget(l, i%7, j);
       }
       ui->playerDetailsGrid->setRowStretch(numRows, 1);
    }
       else
           for(int i=0, j=0; i<playerStats.size(); i++)
           {
               QLabel* l = qobject_cast<QLabel *> (ui->playerDetailsGrid->itemAt(i)->widget());
               l->setText(playerStats[i].first + ": " + playerStats[i].second);

           }



}

void MainWindow::setupChampionDetails()
{
    //tag filter dropdown box
    ui->tagFilterComboBox->addItem("All");
    QVector<QString> tags;
    QMap<QString, QVector<QString>>::iterator i;
     for (i = myChampTags.begin(); i != myChampTags.end(); ++i)
     {
         foreach(QString s , i.value())
             if(!tags.contains(s))
             {
                 tags.append(s);
                 ui->tagFilterComboBox->addItem(s);
             }
     }

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
                    "B Wins" <<
                    "B Losses" <<
                    "P Wins" <<
                    "P Losses" <<
                    "BvP ratio" <<
                    "Total CS" <<
                    "Neutral CS" <<
                    "Minions" <<
                    "Avg CS" <<
                    "Avg N CS" <<
                    "Avg Minions";
    ui->championsTableWidget->setColumnCount(horizHeaders.count());
    ui->championsTableWidget->setHorizontalHeaderLabels(horizHeaders);
    ui->championsTableWidget->setRowCount(mcl.count());

    QStringList vertHeaders;
    vertHeaders << "test1" << "test2";
    ui->championsTableWidget->setVerticalHeaderLabels(vertHeaders);
}


void MainWindow::updateChampionDetails()
{
    for (int i=0; i<ui->championsTableWidget->rowCount(); i++)
    {
        if(ui->tagFilterComboBox->currentText() == "All"
                || ( myChampTags.contains(QString::fromStdString(mcl[i].getName()))
                    && myChampTags[QString::fromStdString(mcl[i].getName())].contains(ui->tagFilterComboBox->currentText()) ))
            ui->championsTableWidget->showRow(i);
    }

}

void MainWindow::displayChampionDetails()
{
    //display champion stats. Display calls must be in same order as headers
    //use MyTableWidgetItem for number variables to ensure proper sorting and use QTableWidgetItem for strings
    ui->championsTableWidget->clearContents();
    int nextOpenIndex = 0;
    int j = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem();
    for (int i=0; i<mcl.count(); i++)
    {
        if(ui->tagFilterComboBox->currentText() == "All"
                || ( myChampTags.contains(QString::fromStdString(mcl[i].getName()))
                    && myChampTags[QString::fromStdString(mcl[i].getName())].contains(ui->tagFilterComboBox->currentText()) ))
        {
            j = 0;
            newItem = new QTableWidgetItem();
            ui->championsTableWidget->setItem(nextOpenIndex, j, new QTableWidgetItem( QString::fromStdString( mcl[i].getName() ) ));
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getKills());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getDeaths());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getAssists());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].KDA() * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getWins());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getLosses());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].winRate(0) * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getBlueWins());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getBlueLosses());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getPurpleWins());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getPurpleLosses());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].winRate(2) * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getCreepScore());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getNeutralCreeps());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,mcl[i].getEnemyMinions());
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].averages(0) * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].averages(1) * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);
            j++;
            newItem = new QTableWidgetItem();
            newItem->setData(0,std::ceil(mcl[i].averages(2) * 100)/ 100);
            ui->championsTableWidget->setItem(nextOpenIndex, j, newItem);

            nextOpenIndex++;
        }
    }

    //ui->championsTableWidget->hideRow(5);
    //std::cout<<mcl[5].getName()<<" hidden\n";

    ui->championsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->championsTableWidget->setVerticalHeaderLabels(vertHeaders);
}

QString boolToString(bool b)
{
    if(b)
        return "True";
    else
        return "False";
}

void MainWindow::displayMatchDetails(int index)
{
    QVector<QPair<QString, QString>> matchStats;
    matchStats.push_back(QPair<QString, QString>( "Match ID", QString::number(mml[index].getID()) ));
    matchStats.push_back(QPair<QString, QString>( "Champ ID", QString::number(mml[index].getChampID()) ));
    matchStats.push_back(QPair<QString, QString>( "Kills", QString::number(mml[index].getKills()) ));
    matchStats.push_back(QPair<QString, QString>( "Deaths", QString::number(mml[index].getDeaths()) ));
    matchStats.push_back(QPair<QString, QString>( "Assists", QString::number(mml[index].getAssists()) ));
    matchStats.push_back(QPair<QString, QString>( "KDA", QString::number(mml[index].KDA()) ));
    matchStats.push_back(QPair<QString, QString>( "CS", QString::number(mml[index].getCreepScore()) ));
    matchStats.push_back(QPair<QString, QString>( "Damage Dealt", QString::number(mml[index].getDamage()) ));
    matchStats.push_back(QPair<QString, QString>( "Multi Kill", QString::number(mml[index].getMultiKill()) ));
    matchStats.push_back(QPair<QString, QString>( "Kill Spree", QString::number(mml[index].getKillSpree()) ));
    matchStats.push_back(QPair<QString, QString>( "Build Cost", QString::number(mml[index].getBuildCost()) ));
    matchStats.push_back(QPair<QString, QString>( "Total Gold", QString::number(mml[index].getGold()) ));

    QString isRankedString = "Normal";
    if(mml[index].isRanked())
        isRankedString = "Ranked";
    QString sideString = "Purple";
    if(mml[index].isBlueSide())
        sideString = "Blue";
    QString winString = "Loss";
    if(mml[index].getResult())
        winString = "Win";
    matchStats.push_back(QPair<QString, QString>(isRankedString, sideString + " " + winString));


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
           ui->gridLayout_2->addWidget(l, i%numRows, j);
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

void MainWindow::on_tagFilterComboBox_currentIndexChanged(const QString &arg1)
{
    ui->championsTableWidget->setSortingEnabled(false);
    displayChampionDetails();
    ui->championsTableWidget->setSortingEnabled(true);
    //ui->championsTableWidget->set
}

void MainWindow::on_graphWindowButton_clicked()
{
   // myNewDialog = new Dialog;
    //myNewDialog->setMainWindow(this);
   //myNewDialog->show();
}
