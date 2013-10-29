#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>

class match
{
public:
	match();
	match(int cID, int i1, int i2, int i3, int i4, int i5, int i6, int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int bc, int cs, bool blue);
	match(int cID, int i1, int i2, int i3, int i4, int i5, int i6, int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int cs, bool blue);
	void setPrices(std::map<int, int>* p) {prices = p; goldSpent();};
	float KDA() {return (kills + assists) / (float)deaths;};
	float BCP() {return 100 * buildCost / (float)gold;};
	void write(std::ofstream out);
	std::string write();
	void printStats();

private:
	int champID, items[6], creepScore, kills, deaths, assists, damageDealt, gold, multiKill, killSpree, id, buildCost;
	bool ranked, result, blueSide;
	std::map<int, int>* prices;
	std::string date, time;
	void goldSpent();
	std::string intToString(int x) {return static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();};
};