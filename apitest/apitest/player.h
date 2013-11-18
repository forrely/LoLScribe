#include <string>
#include <iostream>
#include <map>
#include <sstream>

class player
{
public:
    player();
	player(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, std::map<int, std::pair<int, int> > cPlays, std::map<int, std::pair<int, int> > itemUsage);
	float KDA() {return (kills + assists) / (float)deaths;};
	float winRate(int mode);
	float cWinRate(int ID);
	float iWinRate(int ID);
	float averages(int mode);
	int getKills() {return kills;};
	int getDeaths() {return deaths;};
	int getAssists() {return assists;};
	int getWins() {return wins;};
	int getLosses() {return losses;};
	int getRankedWins() {return rankedWins;};
	int getRankedLosses() {return rankedLosses;};
	int getBlueWins() {return blueWins;};
	int getBlueLosses() {return blueLosses;};
	int getPurpleWins() {return purpleWins;};
	int getPurpleLosses() {return purpleLosses;};
	int getCreepScore() {return creepScore;};
	int getNeutralCreeps() {return neutralCreeps;};
	int getEnemyMinions() {return enemyMinions;};
	std::string getName() {return name;};
	std::string write();
	void setValues(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, std::map<int, std::pair<int, int> > cPlays, std::map<int, std::pair<int, int> > itemUsage);
	void modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue, int id, int i[6]);
	player & operator=(player other) {return *this;};
	

private:
	int kills, deaths, assists, wins, losses, rankedWins, rankedLosses, blueWins, blueLosses, purpleWins, purpleLosses;
	int creepScore, neutralCreeps, enemyMinions;
	std::string name;
	std::map<int, std::pair<int, int> >champPlays; //ID, wins, losses
	std::map<int, std::pair<int, int> >itemsUsed;
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};
