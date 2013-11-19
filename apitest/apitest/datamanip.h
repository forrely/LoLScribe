#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <sstream>
#include <Windows.h>
#include <map>

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
	std::map<int, std::string> champIDs;
	std::map<int, int> prices;
	player activePlayer;
	std::vector<champion> activeChamps;
	std::vector<match> matchHistory;
	std::string activeName;
	static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, std::string *stream);
	int stringToInt(const std::string &str);
	void pullAPI(std::string target, std::string fileName);
	void APICall(std::string playerName, int mode);
	void parseItems();
	void parseChamps();
	void priceList();
	int priceBuild(int items[6]);
	void parseMatches(std::string playerName);
	int operate();
	void loadPlayer(std::string playerName);
	void driver();

private:
	bool firstRun;
};