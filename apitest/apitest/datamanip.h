#ifndef DATAMANIP_H
#define DATAMANIP_H


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <sstream>
#include <Windows.h>
#include <map>
#include <QDir>
#include <QString>
#include <time.h>

#include "champion.h"
#include "match.h"
#include "player.h"

#define MODE_CHAMPION 0
#define MODE_ITEM 1
#define MODE_MATCH 2

class datamanip
{
public:
	std::string tempOut;
    std::map<int, std::string> champNames;
    std::map<std::string, std::vector<std::string>> champTags;
	std::map<int, int> prices;
    std::map<int, std::string> itemNames;
	player activePlayer;
	std::vector<champion> activeChamps;
	std::vector<match> matchHistory;
	std::string activeName;
    std::string myAuthorizationKey;
    datamanip(std::string mAuth);
	static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, std::string *stream);
    const std::string currentDateTime();
	int stringToInt(const std::string &str);
	void pullAPI(std::string target, std::string fileName);
	void APICall(std::string playerName, int mode);
	void parseItems();
	void parseChamps();
    void loadItems();
    void loadChampNames();
    int priceBuild(int items[7]);
	void parseMatches(std::string playerName);
	int operate();
	void loadPlayer(std::string playerName);
	void driver();

private:
	bool firstRun;
};

#endif
