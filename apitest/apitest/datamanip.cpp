#include "datamanip.h"


const std::string datamanip::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

    return buf;
}

size_t datamanip::my_fwrite(void *buffer, size_t size, size_t nmemb, std::string *s)
{
	/*static int first_time = 1;
	static FILE *outFile;
	if (first_time)
	{
		first_time = 0;
		outFile = fopen("temp.txt", "w");
		if (outFile == NULL)
		{
			return -1;
		}
	}
	if (nmemb > 2)
	{
		fwrite(buffer, size, nmemb - 2, outFile);
	}*/
	

	//tempOut += (char*)buffer;
	//tempOut.pop_back();
	//tempOut.pop_back();

	s->append((char*)buffer, size*nmemb);
    //s->pop_back();
	//s->pop_back();
	return size*nmemb;
}

int datamanip::stringToInt(const std::string &str)
{
	std::stringstream ss(str);
	int result;
	return ss >> result ? result : 0;
}

void datamanip::pullAPI(std::string target, std::string fileName)
{
	CURL *curl;
	CURLcode res;
	curl_slist *slist=NULL;
    firstRun = true;

	std::string endpoint = target;

    std::ofstream out;

    out.open(fileName);

//    if(!out.is_open())
//    {
//        std::cout<<"making directory: "<<fileName<<std::endl;
//        QDir().mkpath(QString::fromStdString(fileName));
//        out.open(fileName);
//    }

	slist = curl_slist_append(slist, "X-Mashape-Authorization: jFZRnPb3AD7TXVnfoDlkopqghMrDALtI");
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &datamanip::my_fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tempOut);

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

	/*std::ifstream in("temp.txt");
	while (!in.eof())
	{
		std::string t;
		in >> t;
		tempOut += t;
	}*/
	//std::cout<<tempOut<<std::endl
	out<<tempOut;
	out.close();
	curl_global_cleanup();
    tempOut.clear();
}

void datamanip::APICall(std::string playerName, int mode)
{
	std::string target = "https://teemojson.p.mashape.com/";

    QDir().mkpath("./Players/" + QString::fromStdString(playerName));

	if (mode == MODE_CHAMPION)
	{
		target += "datadragon/champion";
        pullAPI(target, "champsraw.txt");
		parseChamps();
	}
	else if (mode == MODE_ITEM)
	{
		target += "datadragon/item";
		pullAPI(target, "itemsraw.txt");
		parseItems();
	}
	else if (mode == MODE_MATCH)
	{
		target += "player/na/";
		target += playerName;
		target += "/recent_games";
        pullAPI(target, "./Players/" + playerName + "/matchesraw.txt");
	}
	else
	{
		std::cout << "Invalid API request\n";
	}
}

void datamanip::parseItems()
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
                    //std::cout << "Found ID: " << a << " ";
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

				for (size_t i = 2; i < temp.size() - 1; i++)
				{
                    //std::cout << temp[i];
					outFile << temp[i];
				}

                //std::cout << " ";
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
					
                //std::cout << a << "\n";
				outFile << a << "\n";

				seek = "NULL";
			}
		}

		c = inFile.peek();
	}
}

void datamanip::parseChamps()
{
	std::ifstream inFile;
	std::ofstream outFile;
    std::ofstream tagsOutFile;
		
	inFile.open("champsraw.txt");
	outFile.open("champs.txt");
    tagsOutFile.open("champtags.txt");

	int c = inFile.peek();
	int d = 0;
	std::string temp = "NULL";
    std::string tempName = "";
	std::string seek = "\"id\":";
    //std::string tempTag = "";
    //std::map<std::string, std::vector<std::string>> tempChampTags;

	while (c != std::ifstream::traits_type::eof())
	{
		if (seek == "\"id\":")
		{
			inFile >> temp;
				
			if (temp == seek)
			{
				inFile >> temp;
                //std::cout << "Found Champion: ";
				d++;

				for (size_t i = 1; i < temp.size() - 2; i++)
				{
                    //std::cout << temp[i];
					outFile << temp[i];
				}

                //std::cout << " ";
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

				for (size_t i = 1; i < temp.size() - 2; i++)
				{
                    //std::cout << temp[i];
					outFile << temp[i];
				}


                //std::cout << " ";
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
					
				for (size_t i = 2; i < temp.size() - 1; i++)
				{
                    //std::cout << temp[i];
					outFile << temp[i];
				}
                tempName = temp.substr(2,temp.size()-3);

                //std::cout << "\n";
				outFile << "\n";

                seek = "\"tags\":";
                //seek = "\"id\":";
			}
		}
        else if (seek == "\"tags\":")
        {
            inFile >> temp;

            if (temp == seek)
            {
                std::getline(inFile, temp);
                inFile>>temp;

                tagsOutFile << tempName <<"\n"
                            << temp.substr(1,temp.size()-2) << "\n\n";
                //std::cout<<"Tag: " << tempName
                  //               << " " <<temp.substr(1,temp.size()-2) << "\n";
                champTags[tempName].push_back(temp.substr(1,temp.size()-2));

                seek = "\"id\":";
            }
        }

		c = inFile.peek();
	}

	std::cout << "Found " << d << " champions out of 116\n";
}

void datamanip::loadItems()
{
	std::ifstream inFile;
    //std::ofstream outFile, of2, of3;

	inFile.open("items.txt");

    std::string temp = "", temp2 = "", temp3 = "";

	int c = inFile.peek();
	while (c != std::ifstream::traits_type::eof())
	{
		std::getline(inFile, temp, ',');
		std::getline(inFile, temp2, ',');
        std::getline(inFile, temp3);

		int i = stringToInt(temp);

        itemNames[i] = temp2;
        prices[i] = stringToInt(temp3);

		c = inFile.peek();
	}

	inFile.close();
}

int datamanip::priceBuild(int items[7])
{
	int total = 0;
	std::map<int, int>::iterator itr;
    for (int i = 0; i < 7; i++)
	{
		if (items[i] == 2003 || items[i] == 2004 || items[i] == 2009 || items[i] == 2037 || items[i] == 2039 || items[i] == 2042 || items[i] == 2043 || items[i] == 2044 || items[i] == 2050 || items[i] == 0)
		{
			continue;
		}
		else
		{
			itr = prices.find(items[i]);
			total += itr->second;
		}
	}

	return total;
}

void datamanip::parseMatches(std::string playerName)
{
	std::ifstream inFile, if2;
	std::ofstream outFile, of2, of3, of4;

	std::vector<int> matchIDs;
	int c;
	std::string temp = "";
    std::string path = "./Players/" + playerName + "/matchIDs.txt";

	inFile.open(path);
	
	if (inFile.is_open())
	{
		c = inFile.peek();

		while (c != std::ifstream::traits_type::eof())
		{
			std::getline(inFile, temp);

			matchIDs.push_back(stringToInt(temp));
			c = inFile.peek();
		}

		inFile.close();
	}

    inFile.open("champs.txt");
	c = inFile.peek();


	std::map<int, std::pair<int, int> > tempChamps;
	std::pair<int, int> placeholder(0, 0);
	std::map<int, std::pair<int, int> > tempItems;

	while (c != std::ifstream::traits_type::eof())
	{
		std::getline(inFile, temp, ',');
		std::getline(inFile, temp, ',');

		tempChamps[stringToInt(temp)] = placeholder;

		std::getline(inFile, temp);
		c = inFile.peek();
	}

	inFile.close();

	player tempPlayer(playerName, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, tempChamps, tempItems);

	path = "./Players/" + playerName + "/playerStats.txt";
	inFile.open(path);
	std::cout << "Finding " << playerName << "\n";

	if (inFile.is_open())
	{
		std::cout << playerName << " found\n";
		c = inFile.peek();

		std::getline(inFile, temp, ',');
		std::string tempName = temp;
		std::getline(inFile, temp, ',');
		int tempK = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempD = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempA = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempRW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempRL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempBW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempBL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempPW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempPL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempCS = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempNC = stringToInt(temp);
		std::getline(inFile, temp);
		int tempEM = stringToInt(temp);
		
		inFile >> temp;
		//std::getline(inFile, temp, ',');

		std::map<int, std::pair<int, int> > tempCData;

		for (int i = 0; i < 116;  i++)
		{
			std::getline(inFile, temp, ',');
			int t1 = stringToInt(temp);
			std::getline(inFile, temp, ',');
			int t2 = stringToInt(temp);
			std::getline(inFile, temp);
			int t3 = stringToInt(temp);

			std::pair<int, int> tPair(t2, t3);
			tempCData[t1] = tPair;
		}

		std::map<int, std::pair<int, int> > tempIData;


		inFile >> temp;
        c = inFile.peek();
        if (c != std::ifstream::traits_type::eof())
        {
            std::getline(inFile, temp, ',');
        }

		while (c != std::ifstream::traits_type::eof())
		{
			int t1 = stringToInt(temp);
			std::getline(inFile, temp, ',');
			int t2 = stringToInt(temp);
			std::getline(inFile, temp);
			int t3 = stringToInt(temp);

			std::pair<int, int> tPair(t2, t3);
			tempIData[t1] = tPair;

			c = inFile.peek();
            if (c != std::ifstream::traits_type::eof())
            {
                std::getline(inFile, temp, ',');
            }
		}

        tempPlayer.setValues(tempName, tempK, tempD, tempA, tempW, tempL, tempRW, tempRL, tempBW, tempBL, tempPW, tempPL, tempCS, tempNC, tempEM, tempCData, tempIData);


		inFile.close();
	}
		
    inFile.open("./Players/" + playerName+ "/matchesraw.txt");
	std::cout << "Reading data\n";
	path = "./Players/" + playerName + "/matches.txt";
	outFile.open(path, std::ios_base::app);
	
	std::vector<champion> champData;

	c = inFile.peek();
	
	std::string seek = "\"ranked\":";

	int cID = 0, k = 0, d = 0, a = 0, cs = 0, dd = 0, g = 0, ks = 0, mk = 0, mid = 0;
    int nc = 0, em = 0, newMatches = 0, items[7] = {0, 0, 0, 0, 0, 0, 0}, bc = 0;
	bool rank = true, res = true, blue = true;
	std::string dt = "", t = "";
	std::map<int, std::string>::iterator cidItr;

    std::string dataread = "";

	while (c != std::ifstream::traits_type::eof())
	{
		inFile >> temp;
        dataread += " " + temp + " ";

        if (temp == "\"spell2\":" && seek == "\"statType\":")
		{
			seek = "\"createDate\":";
		}

		if (temp == seek)
		{
			if (seek == "\"ranked\":")
			{
				inFile >> temp;
				temp.pop_back();

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

                //std::cout << temp << "\n";
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
				std::string t2 = "";

                //std::cout << temp << "\n";

				for (int i = 0; i < 4; i++)
				{
					inFile >> t2;
				}

				t2.pop_back();

                //std::cout << t2 << "\n";
					
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
                else if (temp == "\"ITEM6\"")
                {
                    items[6] = stringToInt(t2);
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
			}
			else if (seek == "\"createDate\":")
			{
				std::getline(inFile, temp);

                //temp.pop_back();
                //temp.pop_back();

                dt = temp.substr(2, temp.size()-4);

				seek = "\"gameId\":";
			}
			else if (seek == "\"gameId\":")
			{
				inFile >> temp;
				temp.pop_back();

                //std::cout << temp << "\n";

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

                //std::cout << temp << "\n";

				cID = stringToInt(temp);

				seek = "},";
			}
			else if (seek == "},")
			{
				bc = priceBuild(items);
                match test(cID, items, k, d, a, dd, g, mk, ks, mid, rank, res, dt, t, bc, cs, blue);
				tempPlayer.modifyStats(k, d, a, cs, nc, em, res, rank, blue, cID, items);
				outFile << test.write();

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
                    cidItr = champNames.find(cID);
                    //foreach(std::pair<int, std::string> s, champNames)
                      //  std::cout << s.second <<"\n";
                    //cidItr = champNames.begin();
                   //std::cout << cidItr->second << "\n";

                    //std::cout << "iterator data: "<<cidItr->second <<"\n";
                    std::string fileName = "./Players/" + playerName +  "/" + cidItr->second + ".txt";
					if2.open(fileName);

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

	path = "./Players/" + playerName + "/matchIDs.txt";
	of2.open(path, std::ios_base::app);
	path = "./Players/" + playerName + "/playerStats.txt";
	of3.open(path);



    if(!of3.is_open())
    {
        QDir().mkpath(QString::fromStdString(path));
        of3.open(path);
    }

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
        cidItr = champNames.find(champData[i].getID());

		std::string fileName = "./Players/" + playerName + "/" + cidItr->second + ".txt";

		of4.open(fileName);

		of4 << champData[i].write();

		of4.close();
	}

	outFile.close();
	of2.close();
	of3.close();
	inFile.close();

    //if there are new match id's in current matchesraw file, create a backup
    if(newMatches > 0)
    {
        std::ofstream matchesrawBackupFile;
        std::string backupDir = "./Players/" + playerName + "/MatchesRawBackups/";
        std::string path = backupDir + currentDateTime() + "_matchesraw.txt";
        matchesrawBackupFile.open(path);

        std::cout<<"backup path\n"<<path<<std::endl;

        if(!matchesrawBackupFile.is_open())
        {
            QDir().mkpath(QString::fromStdString(backupDir));
            matchesrawBackupFile.open(path);
        }

        std::ifstream srcMatchesraw;
        srcMatchesraw.open("./Players/" + playerName + "/matchesraw.txt");


        matchesrawBackupFile << srcMatchesraw.rdbuf();

        matchesrawBackupFile.close();
        srcMatchesraw.close();
    }

}

int datamanip::operate()
{
	std::cout << "What would you like to do?\n";
	std::cout << "0: Update champion list, 1: Update item list, 2: Pull match history data\n";
	std::cout << "3: Load player stats, 4: Exit\n";

	if (activePlayer.getName() != "NULL")
	{
		std::cout << "Currently loaded player: " << activePlayer.getName() << "\n";
	}

	int i;

	std::cin >> i;

	if (i == 0)
	{
        APICall("NULL", MODE_CHAMPION);
	}
	else if (i == 1)
	{
		APICall("NULL", MODE_ITEM);
	}
	else if (i == 2)
	{
		std::cout << "Enter the player name you would like to search: ";
		std::string tempName;
		std::cin >> tempName;

		std::ifstream inFile("./Players/" + tempName + "/playerStats.txt");
		if (!inFile.is_open())
		{
            //std::string path = "./Players/" + tempName;
            //CreateDirectory(path.c_str(), NULL);
            QString path = "./Players/" + QString::fromStdString(tempName);
            QDir().mkpath(path);

        }
		else
		{
			inFile.close();
		}

		APICall(tempName, MODE_MATCH);
		parseMatches(tempName);
	}
	else if (i == 3)
	{
		std::cout << "Enter the player name you would like to load: ";
		std::string tempName;
		std::cin >> tempName;
		loadPlayer(tempName);
	}
	else if (i == 4)
	{
		return 0;
	}

	std::cout << "\n";

	return 1;
}

void datamanip::loadPlayer(std::string playerName)
{
	if (activeChamps.size() > 0)
	{
		activeChamps.clear();
		matchHistory.clear();
	}

	activeName = playerName;
	
	std::string path = "./Players/" + playerName + "/playerStats.txt";

	std::ifstream inFile(path);

	int c;
	std::string temp = "";

	if (!inFile.is_open())
	{
		std::cout << "Player \"" + playerName + "\" not found.\n";
		return;
	}
	else
	{
		std::cout << playerName << " found.\n";
		std::getline(inFile, temp, ',');
		std::string tempName = temp;
		std::getline(inFile, temp, ',');
		int tempK = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempD = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempA = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempRW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempRL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempBW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempBL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempPW = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempPL = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempCS = stringToInt(temp);
		std::getline(inFile, temp, ',');
		int tempNC = stringToInt(temp);
		std::getline(inFile, temp);
		int tempEM = stringToInt(temp);
		
		inFile >> temp;
		//std::getline(inFile, temp, ',');

		std::map<int, std::pair<int, int> > tempCData;

		for (int i = 0; i < 116;  i++)
		{
			std::getline(inFile, temp, ',');
			int t1 = stringToInt(temp);
			std::getline(inFile, temp, ',');
			int t2 = stringToInt(temp);
			std::getline(inFile, temp);
			int t3 = stringToInt(temp);

			std::pair<int, int> tPair(t2, t3);
			tempCData[t1] = tPair;
		}

		std::map<int, std::pair<int, int> > tempIData;
        inFile >> temp;
		c = inFile.peek();

        if (c != std::ifstream::traits_type::eof())
        {
            std::getline(inFile, temp, ',');
        }

		while (c != std::ifstream::traits_type::eof())
		{
			int t1 = stringToInt(temp);
			std::getline(inFile, temp, ',');
			int t2 = stringToInt(temp);
			std::getline(inFile, temp);
			int t3 = stringToInt(temp);

			std::pair<int, int> tPair(t2, t3);
			tempIData[t1] = tPair;

			c = inFile.peek();
            if (c != std::ifstream::traits_type::eof())
            {
                std::getline(inFile, temp, ',');
            }
		}

		activePlayer.setValues(tempName, tempK, tempD, tempA, tempW, tempL, tempRW, tempRL, tempBW, tempBL, tempPW, tempPL, tempCS, tempNC, tempEM, tempCData, tempIData);

		inFile.close();
	}

	std::map<int, std::string>::iterator cIDItr;

    for (cIDItr = champNames.begin(); cIDItr != champNames.end(); cIDItr++)
	{
		path = "./Players/" + playerName + "/" + cIDItr->second + ".txt";

		inFile.open(path);

		if (inFile.is_open())
		{
			std::cout << "Loading stats for " << cIDItr->second << ".\n";
			std::string t3 = "";
			std::getline(inFile, t3, ',');
			std::string tempN = t3;
			std::getline(inFile, t3, ',');
			int tempID = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempK = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempD = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempA = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempW = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempL = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempRW = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempRL = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempBW = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempBL = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempPW = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempPL = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempCS = stringToInt(t3);
			std::getline(inFile, t3, ',');
			int tempNC = stringToInt(t3);
			std::getline(inFile, t3);
			int tempEM = stringToInt(t3);

			c = inFile.peek();

			std::map<int, std::pair<int, int> > tempItems;

			if (c != std::ifstream::traits_type::eof())
			{
				std::getline(inFile, t3, ',');
				int x = stringToInt(t3);
				std::getline(inFile, t3, ',');
				int y = stringToInt(t3);
				std::getline(inFile, t3);
				int z = stringToInt(t3);

				std::pair<int, int> p(y, z);
				tempItems[x] = p;
			}

			champion tempChamp(tempN, tempK, tempD, tempA, tempW, tempL, tempRW, tempRL, tempBW, tempBL, tempPW, tempPL, tempCS, tempNC, tempEM, tempID, tempItems);

			activeChamps.push_back(tempChamp);
						
			inFile.close();
		}
	}

	path = "./players/" + playerName + "/matches.txt";

	inFile.open(path);

	if (inFile.is_open())
	{
		c = inFile.peek();

		int matchesFound = 0;
		
		while (c != std::ifstream::traits_type::eof())
		{
			 inFile >> temp;
			 int gID = stringToInt(temp);
			 inFile >> temp;
			 int tcID = stringToInt(temp);
			 inFile >> temp;
			 int tCS = stringToInt(temp);
			 inFile >> temp;
			 int tK = stringToInt(temp);
			 inFile >> temp;
			 int tD = stringToInt(temp);
			 inFile >> temp;
			 int tA = stringToInt(temp);
			 inFile >> temp;
			 int tDD = stringToInt(temp);
			 inFile >> temp;
			 int tG = stringToInt(temp);
			 inFile >> temp;
			 int tMK = stringToInt(temp);
			 inFile >> temp;
			 int tKS = stringToInt(temp);
			 inFile >> temp;
			 int tBC = stringToInt(temp);
			 inFile >> temp;
			 bool tR = stringToInt(temp);
			 inFile >> temp;
			 bool tW = stringToInt(temp);
			 inFile >> temp;
			 bool tB = stringToInt(temp);
             int tI[7];
			 inFile >> temp;
			 tI[0] = stringToInt(temp);
			 inFile >> temp;
			 tI[1] = stringToInt(temp);
			 inFile >> temp;
			 tI[2] = stringToInt(temp);
			 inFile >> temp;
			 tI[3] = stringToInt(temp);
			 inFile >> temp;
			 tI[4] = stringToInt(temp);
             inFile >> temp;
			 tI[5] = stringToInt(temp);
             std::getline(inFile, temp);
             tI[6] = stringToInt(temp);
			 std::getline(inFile, temp);
			 std::string tDate = temp;

			 match tMatch(tcID, tI, tK, tD, tA, tDD, tG, tMK, tKS, gID, tR, tW, tDate, "", tBC, tCS, tB);

			 matchHistory.push_back(tMatch);
			 matchesFound++;

			 c = inFile.peek();
		}

		std::cout << "Found " << matchesFound << " matches in match history.\n";
	}
}

void datamanip::loadChampNames()
{
    std::ifstream inFile;

    inFile.open("champs.txt");

    int c = inFile.peek();
    std::string temp = "", temp2 = "", trash = "";


    //std::cout<<"champNames values"<<"\n";
    while (c != std::ifstream::traits_type::eof())
    {
        std::getline(inFile, temp, ',');
        std::getline(inFile, temp2, ',');
        std::getline(inFile, trash);

        int i = stringToInt(temp2);

        champNames[i] = temp;
        //std::cout<<champNames[i]<<"\n";

        c = inFile.peek();
    }

    inFile.close();
}

void datamanip::driver()
{
	std::ifstream inFile;

	inFile.open("champs.txt");

	int c = inFile.peek();
	std::string temp = "", temp2 = "", trash = "";


    std::cout<<"champNames values"<<"\n";
	while (c != std::ifstream::traits_type::eof())
	{
		std::getline(inFile, temp, ',');
		std::getline(inFile, temp2, ',');
		std::getline(inFile, trash);

		int i = stringToInt(temp2);

        champNames[i] = temp;
        std::cout<<champNames[i]<<"\n";

		c = inFile.peek();
	}

	inFile.close();

    loadItems();

//	while (true)
//	{
//		if (operate() == 0)
//		{
//			break;
//		}
//	}
}
