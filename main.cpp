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
std::map<int, std::string> champNames;
QMap<QString, QVector<QString>> champTags;
QMap<int, QString> itemNames;

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

    return (champTags.count() >= champNames.size());

}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    datamanip myDataManip;
    myDataManip.APICall("forrely", MODE_CHAMPION);
    myDataManip.parseChamps();
    myDataManip.loadChampNames();

    myDataManip.APICall("forrely", MODE_ITEM);
    myDataManip.parseItems();
    myDataManip.loadItems();

    //myDataManip.APICall("forrely", MODE_MATCH);
    //myDataManip.parseMatches("forrely");
    //myDataManip.loadPlayer("forrely");

    champNames = myDataManip.champNames;
    w.champNames = champNames;

    //attempt to read champtags from file. if unable to, get champ tags from datamanip class
    if(!readChampTags())
    {
        for(std::map<std::string, std::vector<std::string> >::iterator i = myDataManip.champTags.begin();
            i != myDataManip.champTags.end(); i++)
        {
            for(int j=0; j < i->second.size(); j++)
                champTags[QString::fromStdString(i->first)].push_back( QString::fromStdString(i->second[j]) );
        }

        //foreach(QVector<QString> v, champTags)
          //  std::cout<< v[0].toStdString() <<std::endl;
    }

    for(std::map<int, std::string>::iterator i = myDataManip.itemNames.begin();
        i != myDataManip.itemNames.end(); i++)
        itemNames[i->first] = QString::fromStdString(i->second);

    w.itemNames = itemNames;
    w.setChampTags(champTags);
    w.displayCurrentSummonerData();
    w.show();

    return a.exec();

    return 0;
}

