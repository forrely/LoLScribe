#include "match.h"

match::match(int cID, int i1, int i2, int i3, int i4, int i5, int i6, int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int cs, bool blue)
{
	champID = cID;
	items[0] = i1;
	items[1] = i2;
	items[2] = i3;
	items[3] = i4;
	items[4] = i5;
	items[5] = i6;
	kills = k;
	deaths = d;
	assists = a;
	damageDealt = dd;
	gold = g;
	multiKill = mk;
	killSpree = ks;
	id = mid;
	ranked = rank;
	result = res;
	for (size_t i = 2; i < dt.size(); i++)
	{
		date.push_back(dt[i]);
		//date = dt;
	}
	time = t;
	creepScore = cs;
	buildCost = 0;
	blueSide = blue;
}

match::match(int cID, int i1, int i2, int i3, int i4, int i5, int i6, int k, int d, int a, int dd, int g, int mk, int ks, int mid, bool rank, bool res, std::string dt, std::string t, int bc, int cs, bool blue)
{
	champID = cID;
	items[0] = i1;
	items[1] = i2;
	items[2] = i3;
	items[3] = i4;
	items[4] = i5;
	items[5] = i6;
	kills = k;
	deaths = d;
	assists = a;
	damageDealt = dd;
	gold = g;
	multiKill = mk;
	killSpree = ks;
	id = mid;
	ranked = rank;
	result = res;
	date = dt;
	time = t;
	creepScore = cs;
	buildCost = bc;
	blueSide = blue;
}

void match::goldSpent()
{
	std::map<int, int>::iterator itr;

	for (int i = 0; i < 6; i++)
	{
		if (items[i] == 2043 || items[i] == 2044 || items[i] == 2042 || items[i] == 2050 || items[i] == 2009 || items[i] ==	2003 || items[i] == 2004 || items[i] == 2037 || items[i] == 2039 || items[i] == 2041 || items[i] == 0)
		{
			continue;
		}

		itr = prices->find(items[i]);
		buildCost += itr->second;
	}
}

void match::write(std::ofstream out)
{
	out << id << "\n";

	out << champID << " " << creepScore << " " << kills << " " << deaths << " " << assists << " " << damageDealt << " ";
	out << gold << " " << multiKill << " " << killSpree << " " << buildCost << " " << ranked << " " << result;
	
	for (int i = 0; i < 6; i++)
	{
		out << " " << items[i];
	}

	out << "\n";

	out << date << "," << time << "\n\n";
}

std::string match::write()
{
	std::string temp = "";

	temp += intToString(id);
	temp += "\n";
	temp += intToString(champID) + " ";
	temp += intToString(creepScore) + " ";
	temp += intToString(kills) + " ";
	temp += intToString(deaths) + " ";
	temp += intToString(assists) + " ";
	temp += intToString(damageDealt) + " ";
	temp += intToString(gold) + " ";
	temp += intToString(multiKill) + " ";
	temp += intToString(killSpree) + " ";
	temp += intToString(buildCost) + " ";
	temp += intToString(ranked) + " ";
	temp += intToString(result) + " " + intToString(blueSide);
	
	for (int i = 0; i < 6; i++)
	{
		temp += " " + intToString(items[i]);
	}

	temp += "\n";

	temp += date + time + "\n";

	return temp;
}

void match::printStats()
{
	std::cout << "Champion ID: " << champID << "\n";
	
	for (int i = 0; i < 6; i++)
	{
		 std::cout << "Item " << i + 1 << ": " << items[i] << "\n";
	}

	std::cout << "Creep Score: " << creepScore << "\n";
	std::cout << "Kills: " << kills << "\n";
	std::cout << "Deaths: " << deaths << "\n";
	std::cout << "Assists: " << assists << "\n";
	std::cout << "Damage Dealt: " << damageDealt << "\n";
	std::cout << "Gold Earned: " << gold << "\n";
	std::cout << "Largest Multikill: " << multiKill << "\n";
	std::cout << "Largest Killing Spree: " << killSpree << "\n";
	std::cout << "Match ID: " << id << "\n";
}