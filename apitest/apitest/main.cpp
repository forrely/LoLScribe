//Kai VanDrunen and Themos Panotopoulos


#include <iostream>
#include <curl/curl.h>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <map>
#include <vector>

#include "match.h"
#include "player.h"
#include "champion.h"

std::string tempOut;
std::map<int, std::string> champIDs;
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

void pullAPI(std::string playerName)
{
	CURL *curl;
	CURLcode res;
	curl_slist *slist=NULL;
	//std::string tempOut = "blah";

	std::string endpoint = "https://teemojson.p.mashape.com/player/na/" + playerName + "/recent_games";

	std::ofstream out;
	//out = fopen ("scribeout.txt", "w");
	out.open("matchesraw.txt");

	slist = curl_slist_append(slist, "X-Mashape-Authorization: jFZRnPb3AD7TXVnfoDlkopqghMrDALtI");
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
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


	//std::cout<<tempOut<<std::endl;
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

void parseMatches(std::string playerName)
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

	while (c != std::ifstream::traits_type::eof())
	{
		std::getline(inFile, temp, ',');
		std::getline(inFile, temp, ',');

		tempChamps[stringToInt(temp)] = placeholder;

		std::getline(inFile, temp);
		c = inFile.peek();
	}

	inFile.close();

	player tempPlayer(playerName, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, tempChamps);

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

		/*while (temp != "items")
		{
			std::cout << temp << "\n";
			int t1 = stringToInt(temp);
			std::getline(inFile, temp, ',');
			int t2 = stringToInt(temp);
			std::getline(inFile, temp);
			int t3 = stringToInt(temp);

			std::pair<int, int> tPair(t2, t3);
			tempCData[t1] = tPair;

			std::getline(inFile, temp, ',');
		}*/

		std::map<int, std::pair<int, int> > tempIData;
		c = inFile.peek();

		inFile >> temp;

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
		}

		player tPlayer(tempName, tempK, tempD, tempA, tempW, tempL, tempRW, tempRL, tempBW, tempBL, tempPW, tempPL, tempCS, tempNC, tempEM, tempCData);
		tempPlayer = tPlayer;

		inFile.close();
	}
		
	inFile.open("matchesraw.txt");
	std::cout << "Reading data\n";
	path = "./Players/" + playerName + "/matches.txt";
	outFile.open(path, std::ios_base::app);
	
	std::vector<champion> champData;

	c = inFile.peek();
	
	std::string seek = "\"ranked\":";

	int cID = 0, k = 0, d = 0, a = 0, cs = 0, dd = 0, g = 0, ks = 0, mk = 0, mid = 0;
	int nc = 0, em = 0, newMatches = 0, items[6] = {0, 0, 0, 0, 0, 0};
	bool rank = true, res = true, blue = true;
	std::string dt = "", t = "";
	std::map<int, std::string>::iterator cidItr;

	while (c != std::ifstream::traits_type::eof())
	{
		inFile >> temp;

		if (temp == "\"spell2\":")
		{
			seek = "\"createDate\":";
		}

		if (temp == seek)
		{
			if (seek == "\"ranked\":")
			{
				inFile >> temp;
				temp.pop_back();

				std::cout << temp << "\n";

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

				std::cout << temp << "\n";
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

				std::cout << temp;

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

				std::cout << temp << "\n";

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
			}
			else if (seek == "\"createDate\":")
			{
				std::getline(inFile, temp);

				temp.pop_back();
				temp.pop_back();

				dt = temp;

				seek = "\"gameId\":";
			}
			else if (seek == "\"gameId\":")
			{
				inFile >> temp;
				temp.pop_back();

				std::cout << temp << "\n";

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

				std::cout << temp << "\n";

				cID = stringToInt(temp);

				seek = "},";
			}
			else if (seek == "},")
			{
				match test(cID, items, k, d, a, dd, g, mk, ks, mid, rank, res, dt, t, cs, blue);
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
					cidItr = champIDs.find(cID);
					std::string fileName = "./Players/" + playerName + "/" + cidItr->second + ".txt";
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

		std::string fileName = "./Players/" + playerName + "/" + cidItr->second + ".txt";

		of4.open(fileName);

		of4 << champData[i].write();

		of4.close();
	}

	outFile.close();
	of2.close();
	of3.close();
	inFile.close();
}

int main()
{
	int mode = 0;
	std::ifstream inFile;
	std::string playerName;

	inFile.open("champs.txt");

	int c = inFile.peek();
	std::string temp = "", temp2 = "", trash = "";

	while (c != std::ifstream::traits_type::eof())
	{
		std::getline(inFile, temp, ',');
		std::getline(inFile, temp2, ',');
		std::getline(inFile, trash);

		int i = stringToInt(temp2);

		champIDs[i] = temp;

		c = inFile.peek();
	}

	inFile.close();

	std::cin >> playerName;

	inFile.open("./Players/" + playerName + "/playerStats.txt");

	if (!inFile.is_open())
	{
		std::string path = "./Players/" + playerName;
		CreateDirectory(path.c_str(), NULL);
	}
	else
	{
		inFile.close();
	}

	pullAPI(playerName);
	//parseChamps();
	parseMatches(playerName);

	if (mode == 5)
	{
		//CreateDirectory("./Players", NULL);
		//CreateDirectory("./Players/Mermigas", NULL);
		return 0;
	}

	return 0;
}