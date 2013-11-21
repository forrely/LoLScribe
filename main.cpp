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

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{

    /*std::ofstream *out = (std::ofstream*)stream;
    out->open("scribeout1.txt");

    out->write((char*)buffer, size);
    return nmemb*size;*/
    tempOut += (char*)buffer;
   tempOut.pop_back();
   tempOut.pop_back();
    //tempOut.push_back((char*)buffer);
    //out << (char*)buffer;
    return size*nmemb;
}


int stringToInt(const std::string &str)
{
    std::stringstream ss(str);
    int result;
    return ss >> result ? result : 0;
}

void pullAPI()
{
    CURL *curl;
    CURLcode res;
    curl_slist *slist=NULL;
    //std::string tempOut = "blah";

    std::ofstream out;
    //out = fopen ("scribeout.txt", "w");
    out.open("champsraw.txt");

    slist = curl_slist_append(slist, "X-Mashape-Authorization: jFZRnPb3AD7TXVnfoDlkopqghMrDALtI");
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://teemojson.p.mashape.com/datadragon/champion");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);

    #ifdef SKIP_PEER_VERIFICATION
        /*
            * If you want to connect to a site who isn't using a certificate that is
            * signed by one of the certs in the CA bundle you have, you can skip the
            * verification of the server's certificate. This makes the connection
            * A LOT LESS SECURE.
            *
            * If you have a CA cert for the server stored someplace else than in the
            * default bundle, then the CURLOPT_CAPATH option might come handy for
            * you.
            */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    #endif

    #ifdef SKIP_HOSTNAME_VERIFICATION
        /*
            * If the site you're connecting to uses a different host name that what
            * they have mentioned in their server certificate's commonName (or
            * subjectAltName) fields, libcurl will refuse to connect. You can skip
            * this check, but this will make the connection less secure.
            */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    #endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }


    std::cout<<tempOut<<std::endl;
    out<<tempOut;
    out.close();
    curl_global_cleanup();
}

void parseItems()
{
    std::ifstream inFile;
    std::ofstream outFile;

    inFile.open("itemsraw.txt");
    outFile.open("items.txt");

    int c = inFile.peek();
    std::string temp = "NULL";
    std::string seek = "NULL";

    while (c != std::ifstream::traits_type::eof())
    {
        if (seek == "NULL")
        {
            inFile >> temp;
            if (temp.size() == 7)
            {
                std::string temp2 = "";
                for (int i = 1; i < 5; i++)
                {
                    temp2.push_back(temp[i]);
                }

                int a = stringToInt(temp2);

                if (a > 1000 && a < 9999)
                {
                    std::cout << "Found ID: " << a << " ";
                    outFile << a << ",";
                    seek = "\"name\":";
                }
            }
        }
        else if (seek == "\"name\":")
        {
            inFile >> temp;

            if (temp == seek)
            {
                std::getline(inFile, temp, ',');

                for (int i = 2; i < temp.size() - 1; i++)
                {
                    std::cout << temp[i];
                    outFile << temp[i];
                }

                std::cout << " ";
                outFile << ",";

                seek = "\"total\":";
            }
        }
        else
        {
            inFile >> temp;

            if (temp == seek)
            {
                inFile >> temp;

                int a = stringToInt(temp);

                std::cout << a << "\n";
                outFile << a << "\n";

                seek = "NULL";
            }
        }

        c = inFile.peek();
    }
}

void parseChamps()
{
    std::ifstream inFile;
    std::ofstream outFile;

    inFile.open("champsraw.txt");
    outFile.open("champs.txt");

    int c = inFile.peek();
    int d = 0;
    std::string temp = "NULL";
    std::string seek = "\"id\":";

    while (c != std::ifstream::traits_type::eof())
    {
        if (seek == "\"id\":")
        {
            inFile >> temp;

            if (temp == seek)
            {
                inFile >> temp;
                std::cout << "Found Champion: ";
                d++;

                for (int i = 1; i < temp.size() - 2; i++)
                {
                    std::cout << temp[i];
                    outFile << temp[i];
                }

                std::cout << " ";
                outFile << ",";

                seek = "\"key\":";
            }
        }
        else if (seek == "\"key\":")
        {
            inFile >> temp;

            if (temp == seek)
            {
                inFile >> temp;

                for (int i = 1; i < temp.size() - 2; i++)
                {
                    std::cout << temp[i];
                    outFile << temp[i];
                }

                std::cout << " ";
                outFile << ",";

                seek = "\"name\":";
            }
        }
        else if (seek == "\"name\":")
        {
            inFile >> temp;

            if (temp == seek)
            {
                std::getline(inFile, temp, ',');

                for (int i = 2; i < temp.size() - 1; i++)
                {
                    std::cout << temp[i];
                    outFile << temp[i];
                }

                std::cout << "\n";
                outFile << "\n";

                seek = "\"id\":";
            }
        }

        c = inFile.peek();
    }

    std::cout << "Found " << d << " champions out of 116\n";
}

std::map<int, int> priceList()
{
    std::ifstream inFile;
    std::ofstream outFile, of2, of3;

    std::map<int, int> prices;

    inFile.open("items.txt");

    std::string temp = "", temp2 = "";

    int c = inFile.peek();
    while (c != std::ifstream::traits_type::eof())
    {
        std::getline(inFile, temp, ',');
        std::getline(inFile, temp2, ',');
        std::getline(inFile, temp2);

        int i = stringToInt(temp);

        prices[i] = stringToInt(temp2);

        c = inFile.peek();
    }

    inFile.close();
    return prices;
}

void parseMatches()
{
    std::ifstream inFile, if2;
    std::ofstream outFile, of2, of3, of4;

    std::vector<int> matchIDs;

    inFile.open("./Players/Mermigas/matchIDs.txt");

    int c = inFile.peek();
    std::string temp = "";

    while (c != std::ifstream::traits_type::eof())
    {
        std::getline(inFile, temp);

        matchIDs.push_back(stringToInt(temp));
        c = inFile.peek();
    }

    inFile.close();

    inFile.open("champs.txt");
    c = inFile.peek();

    std::map<int, std::pair<int, int> > tempChamps;
    std::pair<int, int> placeholder(0, 0);

    while (c != std::ifstream::traits_type::eof())
    {
        std::getline(inFile, temp, ',');
        std::getline(inFile, temp, ',');

        tempChamps[stringToInt(temp)] = placeholder;

        std::getline(inFile, temp);
        c = inFile.peek();
    }

    inFile.close();

    inFile.open("matchesraw.txt");

    outFile.open("./Players/Mermigas/matches.txt", std::ios_base::app);

    player tempPlayer("Mermigas", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, tempChamps, tempChamps);
    std::vector<champion> champData;

    c = inFile.peek();

    std::string seek = "\"ranked\":";

    int cID = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, k = 0, d = 0, a = 0, cs = 0, dd = 0, g = 0, ks = 0, mk = 0, mid = 0;
    int nc = 0, em = 0, newMatches = 0, items[6] = {0, 0, 0, 0, 0, 0};
    bool rank = true, res = true, blue = true;
    std::string dt = "", t = "";
    std::map<int, std::string>::iterator cidItr;


    while (c != std::ifstream::traits_type::eof())
    {
        inFile >> temp;

        if (temp == seek)
        {
            if (seek == "\"ranked\":")
            {
                inFile >> temp;
               temp.pop_back();
                //erase
                //std::cout << temp << "\n";

                if (temp == "true")
                {
                    rank = true;
                }
                else
                {
                    rank = false;
                }

                seek = "\"gameMapId\":";
            }
            else if (seek == "\"gameMapId\":")
            {
                inFile >> temp;
               temp.pop_back();

               // std::cout << temp << "\n";
                int i = stringToInt(temp);

                if (i != 1)
                {
                    seek = "\"ranked\":";
                }
                else
                {
                    seek = "\"teamId\":";
                }
            }
            else if (seek == "\"teamId\":")
            {
                inFile >> temp;
               temp.pop_back();

                //std::cout << temp;

                if (stringToInt(temp) == 100)
                {
                    blue = true;
                }
                else
                {
                    blue = false;
                }

                seek = "\"statType\":";
            }
            else if (seek == "\"statType\":")
            {
                inFile >> temp;
               temp.pop_back();
               //temp.pop_back();
                std::string t2 = "";

                std::string s ="\"WARD_PLACED\"";
                std::cout << temp << "\n";
                std::cout<< temp.compare(s)<<"\n";
                std::cout<<s.size()<< " and "<<temp.size()<<std::endl;
//                for(int j=0;j<s.size() && j<temp.size();j++)
//                {
//                    std::cout<<static_cast<int>(temp[j])<<"\n";
//                    std::cout<<static_cast<int>(s[j])<<"\n";
//                }



                for (int i = 0; i < 4; i++)
                {
                    inFile >> t2;
                }

               t2.pop_back();

                std::cout << t2 << "\n";

                if (temp == "\"ITEM0\"")
                {
                    items[0] = stringToInt(t2);
                }
                else if (temp == "\"ITEM1\"")
                {
                    items[1] = stringToInt(t2);
                }
                else if (temp == "\"ITEM2\"")
                {
                    items[2] = stringToInt(t2);
                }
                else if (temp == "\"ITEM3\"")
                {
                    items[3] = stringToInt(t2);
                }
                else if (temp == "\"ITEM4\"")
                {
                    items[4] = stringToInt(t2);
                }
                else if (temp == "\"ITEM5\"")
                {
                    items[5] = stringToInt(t2);
                }
                else if (temp == "\"NUM_DEATHS\"")
                {
                    d = stringToInt(t2);
                }
                else if (temp == "\"TOTAL_DAMAGE_DEALT\"")
                {
                    dd = stringToInt(t2);
                }
                else if (temp == "\"WIN\"")
                {
                    res = true;
                }
                else if (temp == "\"LOSE\"")
                {
                    res = false;
                }
                else if (temp == "\"GOLD_EARNED\"")
                {
                    g = stringToInt(t2);
                }
                else if (temp == "\"CHAMPIONS_KILLED\"")
                {
                    k = stringToInt(t2);
                }
                else if (temp == "\"LARGEST_MULTI_KILL\"")
                {
                    mk = stringToInt(t2);
                }
                else if (temp == "\"LARGEST_KILLING_SPREE\"")
                {
                    ks = stringToInt(t2);
                }
                else if (temp == "\"ASSISTS\"")
                {
                    a = stringToInt(t2);
                }
                else if (temp == "\"MINIONS_KILLED\"")
                {
                    cs += stringToInt(t2);
                    em = stringToInt(t2);
                }
                else if (temp == "\"NEUTRAL_MINIONS_KILLED\"")
                {
                    cs += stringToInt(t2);
                    nc = stringToInt(t2);
                }
                else if (temp ==  "\"WARD_PLACED\"")
                {
                    seek = "\"createDate\":";
                }
            }
            else if (seek == "\"createDate\":")
            {
                std::getline(inFile, temp);
                 std::cout << temp << "\n";

               temp.pop_back();
               temp.pop_back();

                dt = temp;

                seek = "\"gameId\":";
            }
            else if (seek == "\"gameId\":")
            {
                inFile >> temp;
               temp.pop_back();

               std::cout <<"gameid "<< temp << "\n";

                bool dup = false;
                int i = stringToInt(temp);

                for (size_t j = 0; j < matchIDs.size(); j++)
                {
                    if (matchIDs[j] == i)
                    {
                        dup = true;
                    }
                }

                if (dup)
                {
                    seek = "\"ranked\":";
                    cs = 0;
                }
                else
                {
                    mid = i;
                    matchIDs.push_back(i);

                    newMatches++;

                    seek = "\"championId\":";
                }
            }
            else if (seek == "\"championId\":")
            {
                inFile >> temp;

                std::cout <<"champ id "<< temp << "\n";

                cID = stringToInt(temp);

                seek = "},";
            }
            else if (seek == "},")
            {
                match test(cID, items, k, d, a, dd, g, mk, ks, mid, rank, res, dt, t, cs, blue);
                tempPlayer.modifyStats(k, d, a, cs, nc, em, res, rank, blue, cID, items);
                outFile << test.write();

                std::cout<<"wrote match: " <<test.getID()<<std::endl;

                bool active = false;

                for (size_t i = 0; i < champData.size(); i++)
                {
                    if (cID == champData[i].getID())
                    {
                        active = true;
                        champData[i].modifyStats(k, d, a, cs, nc, em, res, rank, blue, items);
                    }
                }

                if (!active)
                {
                    cidItr = champIDs.find(cID);
                    std::string fileName = "./Players/Mermigas/" + cidItr->second + ".txt";
                    if2.open(fileName.c_str());

                    if (if2.is_open())
                    {
                        std::string t3 = "";
                        std::getline(if2, t3, ',');
                        std::string tempN = t3;
                        std::getline(if2, t3, ',');
                        int tempID = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempK = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempD = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempA = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempW = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempL = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempRW = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempRL = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempBW = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempBL = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempPW = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempPL = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempCS = stringToInt(t3);
                        std::getline(if2, t3, ',');
                        int tempNC = stringToInt(t3);
                        std::getline(if2, t3);
                        int tempEM = stringToInt(t3);

                        c = if2.peek();

                        std::map<int, std::pair<int, int> > tempItems;

                        if (c != std::ifstream::traits_type::eof())
                        {
                            std::getline(if2, t3, ',');
                            int x = stringToInt(t3);
                            std::getline(if2, t3, ',');
                            int y = stringToInt(t3);
                            std::getline(if2, t3);
                            int z = stringToInt(t3);

                            std::pair<int, int> p(y, z);
                            tempItems[x] = p;
                        }

                        champion tempChamp(tempN, tempK, tempD, tempA, tempW, tempL, tempRW, tempRL, tempBW, tempBL, tempPW, tempPL, tempCS, tempNC, tempEM, tempID, tempItems);
                        tempChamp.modifyStats(k, d, a, cs, nc, em, res, rank, blue, items);

                        champData.push_back(tempChamp);
                        std::cout<<"accessed: " <<tempChamp.getName() <<std::endl;

                        if2.close();
                    }
                    else
                    {
                        champion tempChamp(cID, cidItr->second);
                        tempChamp.modifyStats(k, d, a, cs, nc, em, res, rank, blue, items);
                        champData.push_back(tempChamp);
                    }
                }

                cs = 0;

                seek = "\"ranked\":";
            }
        }
        c = inFile.peek();
    }

    of2.open("./Players/Mermigas/matchIDs.txt", std::ios_base::app);
    of3.open("./Players/Mermigas/playerStats.txt");

    of3 << tempPlayer.write();

    if (newMatches < matchIDs.size())
    {
        for (size_t i = matchIDs.size() - (newMatches + 1); i < matchIDs.size(); i++)
        {
            of2 << matchIDs[i] << "\n";
        }
    }
    else
    {
        for (size_t i = 0; i < matchIDs.size(); i++)
        {
            of2 << matchIDs[i] << "\n";
        }
    }

    for (size_t i = 0; i < champData.size(); i++)
    {
        cidItr = champIDs.find(champData[i].getID());

        std::string fileName = "./Players/Mermigas/" + cidItr->second + ".txt";

        of4.open(fileName.c_str());

        of4 << champData[i].write();

        //std::cout<<champData[i].write();

        of4.close();
    }

    outFile.close();
    of2.close();
    of3.close();
    inFile.close();

    if(champData.size() == 0)
        std::cout<<"no champ data given"<<std::endl;
    else
        std::cout<<champData[0].write()<<std::endl;
    curChampList = champData;
}




int main(int argc, char *argv[])
{
//    int mode = 0;
//    std::ifstream inFile;

//    inFile.open("champs.txt");

//    int c = inFile.peek();
//    std::string temp = "", temp2 = "", trash = "";

//    while (c != std::ifstream::traits_type::eof())
//    {
//        std::getline(inFile, temp, ',');
//        std::getline(inFile, temp2, ',');
//        std::getline(inFile, trash);

//        int i = stringToInt(temp2);

//        champIDs[i] = temp;

//        c = inFile.peek();
//    }

//    inFile.close();

//   // pullAPI();
//    //parseChamps();
//    //parseMatches();


//    if (mode == 5)
//    {
//        //CreateDirectory("./Players", NULL);
//        //CreateDirectory("./Players/Mermigas", NULL);
//        return 0;
//    }

    QApplication a(argc, argv);
    MainWindow w;

    datamanip myDataManip;
    myDataManip.driver();
    myDataManip.APICall("forrely", MODE_MATCH);
    myDataManip.parseMatches("forrely");
    myDataManip.loadPlayer("forrely");

    QVector<player> tempPlayers;
    tempPlayers.push_back(myDataManip.activePlayer);
    w.mpl = tempPlayers;

    QVector<champion> tempchamplist;
    foreach(champion c, myDataManip.activeChamps)
    {
       // std::cout<< c.getName()<<"\n";
        tempchamplist.push_back(c);
    }
    w.mcl = tempchamplist;

    QVector<match> tempmatchlist;

    foreach(match m, myDataManip.matchHistory)
    {
       // std::cout<< c.getName()<<"\n";
        tempmatchlist.push_back(m);
    }
    w.mml = tempmatchlist;


    //std::map<int, std::pair<int, int> > tempChamps;

    //w.mp = player("test", 1,2,3,4,5,6,7,8,9,10,11,12,13,14,tempChamps,tempChamps);
//    w.mcl = QVector<champion>();
//    w.mcl.append(champion("testChamp1", 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,tempChamps));
//    w.mcl.append(champion("testChamp2", 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,tempChamps));
//    w.mcl.append(champion("testChamp3", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp4", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp5", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp6", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp9", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp10", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp11", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));
//    w.mcl.append(champion("testChamp12", 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,tempChamps));

    //w.mml = QVector<match>();
    //int testintarray[4] = {0,1,2,3};
    //w.mml.append(match(1001,testintarray, 1,2,3,4,5,6,7,8,true,true,"what", "who", 9,10,true));
    //w.mml.append(match(1002,testintarray, 2,3,4,5,6,7,8,9,false,false,"what2", "who2", 10,11,false));


    QMap<QString, QVector<QString>> champTags;
    //load tags from file and or api here <-
    //testing values
    champTags["testChamp1"] = QVector<QString>({"Mage", "Tank"});
    champTags["testChamp2"] = QVector<QString>({"Support"});
    champTags["testChamp3"] = QVector<QString>({"Assassin", "Tank"});

    w.setChampTags(champTags);


    //std::cout << "attempting system command" << std::endl;
    //char* jar = "java -jar test.jar 3.13.13_11_13_11_26 forrely sk2p0peru forrely";
    //std::cout << exec(jar) << std::endl;

    w.displayData();
    w.show();

    return a.exec();

    return 0;
}

