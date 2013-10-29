#include "champion.h"

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

champion::champion(std::string n, int k, int d, int a, int w, int l, int rw, int rl, int bw, int bl, int pw, int pl, int cs, int nc, int em, int idt)
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
}

float champion::winRate(int mode)
{
	switch (mode)
	{
	case 0:
		return (float)wins / (wins + losses);

	case 1:
		return (float)rankedWins / (rankedWins + rankedLosses);

	case 2:
		return (float)blueWins / (blueWins + blueLosses);

	case 3:
		return (float)purpleWins / (purpleWins + purpleLosses);

	default:
		return (float)wins / (wins + losses);
	}
}

float champion::averages(int mode)
{
	switch (mode)
	{
	case 0:
		return (float)creepScore / (wins + losses);

	case 1:
		return (float)neutralCreeps / (wins + losses);

	case 2:
		return (float)enemyMinions / (wins + losses);

	case 3:
		return (float)kills / (wins + losses);

	case 4:
		return (float)deaths / (wins + losses);

	case 5:
		return (float)assists / (wins + losses);

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

	return temp;
}

void champion::modifyStats(int k, int d, int a, int cs, int nc, int em, bool win, bool ranked, bool blue)
{
	kills += k;
	deaths += d;
	assists += a;
	creepScore += cs;
	neutralCreeps += nc;
	enemyMinions += em;

	if (win)
	{
		wins++;

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