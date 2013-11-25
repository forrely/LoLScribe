#include "mainwindow.h"
#include "ui_mainwindow.h"

//custom tablewidgetitem class to allow sorting by numeric value
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
//    QSystemTrayIcon* trayIcon = new QSystemTrayIcon();
//    trayIcon->setIcon(QIcon("Resources/draven icon.png"));
//    trayIcon->show();

//    QAction* closeAction = new QAction("Close", this);
//    QMenu* trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(closeAction);
//    trayIcon->setContextMenu(trayIconMenu);

//    QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(trayIconCloseAction_triggered()));

    createActions();
        createTrayIcon();
        trayIcon->setIcon(QIcon(QString::fromUtf8("myappico.ico")));
        trayIcon->show();
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason )),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


        myDataManip = new datamanip();
    ui->graphWindowButton->hide();
    ui->playerComboBox->hide();



    //ui->label->setText(QString::fromStdString(myPlayer->name));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(downloadMatchesAction);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("LolScribe"));
//    connect(
//               trayIcon,
//             SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//               this,
//               SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason))
//              );

}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason)
{
}

void MainWindow::showHideWindow()
{
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    downloadMatchesAction = new QAction(tr("&Download Matches"), this);
    connect(downloadMatchesAction, SIGNAL(triggered()), this, SLOT(downloadCurrentPlayerMatches()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("LOLSCRIBE"), tr("Some msg"), icon, 100);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        show();
        break;
        case QSystemTrayIcon::DoubleClick:
        show();
        break;
        case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
        default:;
    }
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
    //Champion data tab display
    setupChampionDetails();
    displayChampionDetails();


    //Display match history tab
    //display match list
    ui->matchListWidget->clear();

    for(int i=0; i<mml.count(); i++)
    {
        ui->matchListWidget->addItem(new QListWidgetItem( "match " + QString::number(mml[i].getID()) ));
    }
    ui->matchListWidget->setCurrentRow(0);

    //display match details
    displayMatchDetails(ui->matchListWidget->currentRow());

    //display player tab
    displayPlayerDetails(0);
}


bool champPlaysSort(const QPair<QString, int> c1, const QPair<QString, int> c2)
{
    return (c1.second > c2.second);
}

QString MainWindow::champIconFileName(int i)
{
    return "./Resources/champions/" + QString::fromStdString(champNames[i]) + "_Square_0.png";
}
QString MainWindow::champIconFileName(QString s)
{
    return "./Resources/champions/" + s + "_Square_0.png";
}

void MainWindow::displayPlayerDetails(int index)
{
    //clear favorite champions list and populate it with icons and text
     ui->favoriteChampsList->clear();
     int numFavChamps = 5;
     QVector<QPair<QString, int>> champPlays;

     //load champ plays into a vector of pairs
     for(std::map<int, std::string>::iterator i = champNames.begin();
         i != champNames.end(); i++)
     {
         champPlays.push_back(
                     QPair<QString, int>(QString::fromStdString(i->second), mpl[index].cPlays(i->first)) );
     }
     //sort champions by game amounts and display
     qSort(champPlays.begin(), champPlays.end(), champPlaysSort);
     for(int i=0; i<numFavChamps; i++)
     {
         if(champPlays[i].second>0)
         {
             ui->favoriteChampsList->addItem(
                         new QListWidgetItem(QIcon(champIconFileName(champPlays[i].first)), champPlays[i].first));
             std::cout<<champIconFileName(champPlays[i].first).toStdString()<<std::endl;
         }
         else
             i = numFavChamps;
     }


     //display player stats
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
    playerStats.push_back(QPair<QString, QString>("Avg Kills",QString::number(std::ceil(mpl[index].averages(3)))));
    playerStats.push_back(QPair<QString, QString>("Avg Deaths",QString::number(std::ceil(mpl[index].averages(4)))));
    playerStats.push_back(QPair<QString, QString>("Avg Assists",QString::number(std::ceil(mpl[index].averages(5)))));
    playerStats.push_back(QPair<QString, QString>("KDA",QString::number(std::ceil( mpl[index].KDA() * 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("Avg CS",QString::number(std::ceil(mpl[index].averages(0)* 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("Avg N CS",QString::number(std::ceil(mpl[index].averages(1)* 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("Avg E CS",QString::number(std::ceil(mpl[index].averages(2)* 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("Normal Winrate",QString::number(std::ceil(mpl[index].winRate(0)* 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("BvP Winrate",QString::number(std::ceil(mpl[index].winRate(2)* 100)/ 100)));
    playerStats.push_back(QPair<QString, QString>("Ranked Winrate",QString::number(std::ceil(mpl[index].winRate(1)* 100)/ 100)));
     ui->playerNameLabel->setText(QString::fromStdString(mpl[index].getName()));  //displaying player name in label

     //number of rows to display player stats in
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
           if(i%numRows == 0)
               j++;
           l = new QLabel(playerStats[i].first + ": " + playerStats[i].second);
           l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
           l->setTextInteractionFlags(Qt::TextSelectableByMouse);
           ui->playerDetailsGrid->addWidget(l, i%numRows, j);
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
    if(mml.size() != 0)
    {

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

        QString s = "./Resources/champions/";
            s.append(QString::fromStdString(champNames[mml[index].getChampID()]));
            s.append("_Square_0.png");
            ui->ChampIcon->setPixmap(QPixmap(s));

            if (mml[index].getItem(0) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(0)));
                i.append(".png");
                ui->item1->setPixmap(QPixmap(i));
                ui->item1->setToolTip(itemNames[mml[index].getItem(0)]);
            }

            if (mml[index].getItem(1) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(1)));
                i.append(".png");
                ui->item2->setPixmap(QPixmap(i));
                ui->item2->setToolTip(itemNames[mml[index].getItem(1)]);
            }

            if (mml[index].getItem(2) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(2)));
                i.append(".png");
                ui->item3->setPixmap(QPixmap(i));
                ui->item3->setToolTip(itemNames[mml[index].getItem(2)]);
            }

            if (mml[index].getItem(3) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(3)));
                i.append(".png");
                ui->item4->setPixmap(QPixmap(i));
                ui->item4->setToolTip(itemNames[mml[index].getItem(3)]);
            }

            if (mml[index].getItem(4) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(4)));
                i.append(".png");
                ui->item5->setPixmap(QPixmap(i));
                ui->item5->setToolTip(itemNames[mml[index].getItem(4)]);
            }

            if (mml[index].getItem(5) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(5)));
                i.append(".png");
                ui->item6->setPixmap(QPixmap(i));
                ui->item6->setToolTip(itemNames[mml[index].getItem(5)]);
            }
            if (mml[index].getItem(6) != 0)
            {
                QString i = "./Resources/items/";
                i.append(QString::number(mml[index].getItem(6)));
                i.append(".png");
                ui->item7->setPixmap(QPixmap(i));
                //ui->item7->setText(QString::number( mml[index].getItem(6)));
                ui->item7->setToolTip(itemNames[mml[index].getItem(6)]);
            }
    }


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
           if(i%numRows == 0)
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

void MainWindow::displayCurrentSummonerData()
{
    loadSettings();
    myDataManip = new datamanip();

    myDataManip->driver();
    myDataManip->APICall(workingName.toStdString(), MODE_MATCH);
    myDataManip->parseMatches(workingName.toStdString());
    myDataManip->loadPlayer(workingName.toStdString());

    QVector<player> tempPlayers;
    tempPlayers.push_back(myDataManip->activePlayer);
    mpl = tempPlayers;

    QVector<champion> tempchamplist;
    foreach(champion c, myDataManip->activeChamps)
    {
       // std::cout<< c.getName()<<"\n";
        tempchamplist.push_back(c);
    }
    mcl = tempchamplist;

    QVector<match> tempmatchlist;

    foreach(match m, myDataManip->matchHistory)
    {
       // std::cout<< c.getName()<<"\n";
        tempmatchlist.push_back(m);
    }
    mml = tempmatchlist;

    displayData();
}

void MainWindow::loadSettings()
{
    std::string summonerName = "none";
       std::string summonerServer = "none";
       std::string authorizationKeys = "none";
       std::string applicationpath="none";
       std::ifstream readfile;
       std::string line;
       readfile.open ("user_settings.txt");
       if (readfile.is_open())
         {
       while(std::getline(readfile,line))
       {

           std::cout<<line<<std::endl;
        if (line == "Summoner Name")
           {
            std::cout<<"looking for summoner name"<<std::endl;

            std::getline(readfile,summonerName);


               QString summoner_name=QString::fromStdString(summonerName);
               QFont f = ui->nameEdit->font();
               f.setBold(false);
               ui->nameEdit->setText(summoner_name);
           }
        else if(line=="Summoner Server")
            {
            std::getline(readfile,summonerServer);
            QString server=QString::fromStdString(summonerServer);
            ui->server_edit->setText(server);

            }
        else if(line=="Mashup Authorization Keys")
           {
            std::getline(readfile,authorizationKeys);
            QString keys=QString::fromStdString(authorizationKeys);
            ui->authorizationkeys_edit->setText(keys);

           }
        else if(line=="LOL Application Path")
        {
            std::getline(readfile,applicationpath);
            QString applicationpath_Q=QString::fromStdString(applicationpath);

            ui->application_path->setText(applicationpath_Q);
        }
       }
   }
       readfile.close();

           workingName = QString::fromStdString(summonerName);
           workingserver=QString::fromStdString(summonerServer);
           authorization=QString::fromStdString(authorizationKeys);
           application=QString::fromStdString(applicationpath);
}

void MainWindow::saveSettings()
{

}


//**************
//Slots
//**************

void MainWindow::downloadCurrentPlayerMatches()
{

    std::cout<<"downloading current player matches "<<std::endl;
    myDataManip->APICall(workingName.toStdString(), MODE_MATCH);
    displayCurrentSummonerData();
}

//confirm a name change in the settings tab
void MainWindow::on_nameSetButton_clicked()
{

    workingName = ui->nameEdit->toPlainText();
    workingserver=ui->server_edit->toPlainText();
    authorization=ui->authorizationkeys_edit->toPlainText();
    application=ui->application_path->toPlainText();
    QFont f = ui->nameEdit->font();
    f.setBold(false);
    ui->nameEdit->setFont(f);
    ui->server_edit->setFont(f);
    ui->authorizationkeys_edit->setFont(f);
    ui->application_path->setFont(f);

    // reading and writing from setting file
    std::string q;
    //std::string str = q.toStdString();

    std::ofstream myfile;
    myfile.open ("user_settings.txt");


    QString name_edit=ui->nameEdit->toPlainText();
    std::string nameedit = name_edit.toStdString();
    QString server_edit=ui->server_edit->toPlainText();
    std::string serveredit=server_edit.toStdString();
    QString authorization_edit=ui->authorizationkeys_edit->toPlainText();
    std::string authorizationedit=authorization_edit.toStdString();
    QString applicationpath_edit=ui->application_path->toPlainText();
    std::string applicationpath=applicationpath_edit.toStdString();
    myfile<<"Summoner Name"<<"\n";
    myfile<<nameedit<<"\n";
    myfile<<"Summoner Server"<<"\n";
    myfile<<serveredit<<"\n";
    myfile<<"Mashup Authorization Keys"<<"\n";
    myfile<<authorizationedit<<"\n";
    myfile<<"LOL Application Path"<<"\n";
    myfile<<applicationpath<<"\n";
    myfile.close();

    displayCurrentSummonerData();
}

//cancel a name change in the settings tab
void MainWindow::on_nameCancleButton_clicked()
{
    ui->nameEdit->setText(workingName);
    ui->server_edit->setText(workingserver);
        ui->authorizationkeys_edit->setText(authorization);
        ui->application_path->setText(application);
}

//bold and unbold the name change field in the settings tab to indicate a change was made
void MainWindow::on_nameEdit_textChanged()
{
    QFont f = ui->nameEdit->font();

    f.setBold( ui->nameEdit->toPlainText() != workingName );

    ui->nameEdit->setFont(f);

}

void MainWindow::on_server_edit_textChanged()
{
    QFont f = ui->server_edit->font();

    f.setBold( ui->server_edit->toPlainText() != workingserver);

    ui->server_edit->setFont(f);
}
void MainWindow::on_authorizationkeys_edit_textChanged()
{
    QFont f = ui->authorizationkeys_edit->font();

    f.setBold( ui->authorizationkeys_edit->toPlainText() != authorization);

    ui->authorizationkeys_edit->setFont(f);
}
void MainWindow::on_application_path_textChanged()
{
    QFont f = ui->application_path->font();

    f.setBold( ui->application_path->toPlainText() != application );

    ui->application_path->setFont(f);

}

//display the match stats for the currently selected row in matchListWidget
void MainWindow::on_matchListWidget_currentRowChanged(int currentRow)
{
    if(currentRow >= 0 && mml.count()> currentRow)
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
