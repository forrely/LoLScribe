#include <string>
#include <iostream>
#include <map>
#include <sstream>

class player
{
public:
	player();
	player(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em);
	float KDA() {return (kills + assists) / (float)deaths;};
	float winRate(int mode);
	float averages(int mode);
	std::string write();
	void modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue);

public:
	int kills, deaths, assists, wins, losses, rankedWins, rankedLosses, blueWins, blueLosses, purpleWins, purpleLosses;
	int creepScore, neutralCreeps, enemyMinions;
	std::string name;
	std::map<int, int>champPlays;
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};
