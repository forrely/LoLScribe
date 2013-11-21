#include "champion.h"

champion::champion()
{
    champion(0,"");
}

champion::champion(int idt, std::string n)
{
	kills = 0;
	deaths = 0;
	assists = 0;
	wins = 0;
	losses = 0;
	rankedWins = 0;
	rankedLosses = 0;
	blueWins = 0;
	blueLosses = 0;
	purpleWins = 0;
	purpleLosses = 0;
	creepScore = 0;
	neutralCreeps = 0;
	enemyMinions = 0;
	id = idt;
	name = n;
}

champion::champion(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, int idt, std::map<int, std::pair<int, int> > items)
{
	kills = k;
	deaths = d;
	assists = a;
	wins = w;
	losses = l;
	rankedWins = rw;
	rankedLosses = rl;
	blueWins = bw;
	blueLosses = bl;
	purpleWins = pw;
	purpleLosses = pl;
	creepScore = cs;
	neutralCreeps = nc;
	enemyMinions = em;
	id = idt;
	name = n;
	itemsUsed = items;
}

float champion::winRate(int mode)
{
	switch (mode)
	{
	case 0:
        return (wins+losses == 0 ? 0 : (float)wins / (wins + losses));

	case 1:
        return (rankedWins+rankedLosses == 0 ? 0 : (float)rankedWins / (rankedWins + rankedLosses));

	case 2:
        return (blueWins+blueLosses == 0 ? 0 : (float)blueWins / (blueWins + blueLosses));

	case 3:
        return (purpleWins+purpleLosses == 0 ? 0 : (float)purpleWins / (purpleWins + purpleLosses));

	default:
        return (wins+losses == 0 ? 0 : (float)wins / (wins + losses));
	}
}

float champion::averages(int mode)
{
	switch (mode)
	{
	case 0:
        return (wins+losses == 0 ? 0 : (float)creepScore / (wins + losses));

	case 1:
        return (wins+losses == 0 ? 0 : (float)neutralCreeps / (wins + losses));

	case 2:
        return (wins+losses == 0 ? 0 : (float)enemyMinions / (wins + losses));

	case 3:
        return (wins+losses == 0 ? 0 : (float)kills / (wins + losses));

	case 4:
        return (wins+losses == 0 ? 0 : (float)deaths / (wins + losses));

	case 5:
        return (wins+losses == 0 ? 0 : (float)assists / (wins + losses));

	default:
		return -1;
	}
}

std::string champion::write()
{
	std::string temp = "";

	temp += name + ",";
	temp += intToString(id) + ",";
	temp += intToString(kills) + ",";
	temp += intToString(deaths) + ",";
	temp += intToString(assists) + ",";
	temp += intToString(wins) + ",";
	temp += intToString(losses) + ",";
	temp += intToString(rankedWins) + ",";
	temp += intToString(rankedLosses) + ",";
	temp += intToString(blueWins) + ",";
	temp += intToString(blueLosses) + ",";
	temp += intToString(purpleWins) + ",";
	temp += intToString(purpleLosses) + ",";
	temp += intToString(creepScore) + ",";
	temp += intToString(neutralCreeps) + ",";
	temp += intToString(enemyMinions);

	std::map<int, std::pair<int, int> >::iterator itr;

	for (itr = itemsUsed.begin(); itr != itemsUsed.end(); itr++)
	{
		temp += "\n";
		temp += intToString(itr->first) + ",";
		temp += intToString(itr->second.first) + ",";
		temp += intToString(itr->second.second);
	}

	return temp;
}

void champion::modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue, int i[6])
{
	kills += k;
	deaths += d;
	assists += a;
	creepScore += cs;
	neutralCreeps += nc;
	enemyMinions += em;

	std::map<int, std::pair<int, int> >::iterator itr;

	if (win)
	{
		wins++;

		for (int j = 0; j < 6; j++)
		{
			if (i[j] != 0)
			{
				itr = itemsUsed.find(i[j]);

				if (itr != itemsUsed.end())
				{
					itr->second.first++;
				}
				else
				{
					std::pair<int, int> temp(1, 0);
					itemsUsed[i[j]] = temp;
				}
			}
		}

		if (ranked)
		{
			rankedWins++;
		}
		
		if (blue)
		{
			blueWins++;
		}
		else
		{
			purpleWins++;
		}
	}
	else
	{
		losses++;

		for (int j = 0; j < 6; j++)
		{
			if (i[j] != 0)
			{
				itr = itemsUsed.find(i[j]);

				if (itr != itemsUsed.end())
				{
					itr->second.second++;
				}
				else
				{
					std::pair<int, int> temp(0, 1);
					itemsUsed[i[j]] = temp;
				}
			}
		}

		if (ranked)
		{
			rankedLosses++;
		}

		if (blue)
		{
			blueLosses++;
		}
		else
		{
			purpleLosses++;
		}
	}
}

float champion::iWinRate(int ID)
{
	std::map<int, std::pair<int, int> >::iterator itr = itemsUsed.find(ID);

	return (float)(itr->second.first) / (itr->second.first + itr->second.second);
}
