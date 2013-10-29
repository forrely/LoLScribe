#include <iostream>
#include <string>
#include <map>
#include <sstream>

class champion
{
public:
	champion();
	champion(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, int idt);
	champion(int idt, std::string n);
	float KDA() {return (kills + assists) / (float)deaths;};
	float winRate(int mode);
	float averages(int mode);
	std::string write();
	void modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue);
	int getID() {return id;};

private:
	int kills, deaths, assists, wins, losses, rankedWins, rankedLosses, blueWins, blueLosses, purpleWins, purpleLosses;
	int creepScore, neutralCreeps, enemyMinions, id;
	std::string name;
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};