#include <iostream>
#include <string>
#include <map>
#include <sstream>

class champion
{
public:
    champion();
	champion(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, int idt, std::map<int, std::pair<int, int> > items);
	champion(int idt, std::string n);
	float KDA() {return (kills + assists) / (float)deaths;};
	float winRate(int mode);
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
	void modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue, int i[6]);
	int getID() {return id;};

private:
	int kills, deaths, assists, wins, losses, rankedWins, rankedLosses, blueWins, blueLosses, purpleWins, purpleLosses;
	int creepScore, neutralCreeps, enemyMinions, id;
	std::map<int, std::pair<int, int> > itemsUsed; //ID, wins, losses
	std::string name;
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};
