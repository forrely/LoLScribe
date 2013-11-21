#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QVector>
#include <QMap>

#include <stdlib.h>
#include <fstream>

#include <Windows.h>
#include <map>
#include <vector>

#include "apitest/apitest/datamanip.h"

//#include "apitest/apitest/match.h"
//#include "apitest/apitest/player.h"
//#include "apitest/apitest/champion.h"

std::vector<champion> curChampList;

std::string tempOut;
std::map<int, std::string> champIDs;
QMap<QString, QVector<QString>> champTags;

std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

bool readChampTags()
{
    QFile file("champtags.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString cName = in.readLine();
        QString cTag = in.readLine();
        while(cTag != "" && !in.atEnd())
        {
            champTags[cName].push_back(cTag);
            cTag = in.readLine();
        }
    }

    file.close();

    return (champTags.count() >= champIDs.size());

}


int main(int argc, char *argv[])
{
    //QMap<QString, QVector<QString>>
    QApplication a(argc, argv);
    MainWindow w;

    datamanip myDataManip;
//    //myDataManip.APICall("forrely", MODE_CHAMPION);
//    //myDataManip.APICall("forrely", MODE_ITEM);
    myDataManip.driver();
//    //myDataManip.APICall("forrely", MODE_MATCH);
    myDataManip.parseChamps();
//    myDataManip.parseMatches("forrely");
//    myDataManip.loadPlayer("forrely");

    champIDs = myDataManip.champIDs;


//    QVector<player> tempPlayers;
//    tempPlayers.push_back(myDataManip.activePlayer);
//    w.mpl = tempPlayers;

    w.champIDs = champIDs;

//    //give mainwindow our data manip
//    w.myDataManip = &myDataManip;


//    QVector<champion> tempchamplist;
//    foreach(champion c, myDataManip.activeChamps)
//    {
//       // std::cout<< c.getName()<<"\n";
//        tempchamplist.push_back(c);
//    }
//    w.mcl = tempchamplist;

//    QVector<match> tempmatchlist;

//    foreach(match m, myDataManip.matchHistory)
//    {
//       // std::cout<< c.getName()<<"\n";
//        tempmatchlist.push_back(m);
//    }
//    w.mml = tempmatchlist;


    //loading tags from data manip class
    //QMap<QString, QVector<QString>> champTags;

    if(!readChampTags())
    {

        //std::cout<<"first gotten tag: "<<myDataManip.champTags["Annie"][0]<<std::endl;

        for(std::map<std::string, std::vector<std::string> >::iterator i = myDataManip.champTags.begin();
            i != myDataManip.champTags.end(); i++)
        {
            for(int j=0; j < i->second.size(); j++)
                champTags[QString::fromStdString(i->first)].push_back( QString::fromStdString(i->second[j]) );
        }

        foreach(QVector<QString> v, champTags)
            std::cout<< v[0].toStdString() <<std::endl;
    }


    w.setChampTags(champTags);


    //std::cout << "attempting system command" << std::endl;
    //char* jar = "java -jar test.jar 3.13.13_11_13_11_26 forrely sk2p0peru forrely";
    //std::cout << exec(jar) << std::endl;

    w.displayCurrentSummonerData();
    w.show();

    return a.exec();

    return 0;
}

