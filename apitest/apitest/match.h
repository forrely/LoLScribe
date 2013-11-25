#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>

class match
{
public:
	match();
    match(int cID, int i[7], int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int bc, int cs, bool blue);
    match(int cID, int i[7], int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int cs, bool blue);
	void setPrices(std::map<int, int>* p) {prices = p; goldSpent();};
    float KDA() {return ( deaths == 0 ? kills+assists : (kills+assists)/(float)deaths );};
    float BCP() {return ( gold == 0 ? 100 : 100 * buildCost / (float)gold );};
	int getChampID() {return champID;};
	int getItem(int i) {return items[i];};
	int getCreepScore() {return creepScore;};
	int getKills() {return kills;};
	int getDeaths() {return deaths;};
	int getAssists() {return assists;};
	int getDamage() {return damageDealt;};
	int getGold() {return gold;};
	int getMultiKill() {return multiKill;};
	int getKillSpree() {return killSpree;};
	int getID() {return id;};
	int getBuildCost() {return buildCost;};
	bool getResult() {return result;};
	bool isRanked() {return ranked;};
	bool isBlueSide() {return blueSide;};
	void write(std::ofstream out);
	std::string write();
	void printStats();

private:
    int champID, items[7], creepScore, kills, deaths, assists, damageDealt, gold, multiKill, killSpree, id, buildCost;
	bool ranked, result, blueSide;
	std::map<int, int>* prices;
	std::string date, time;
	void goldSpent();
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};

#endif
