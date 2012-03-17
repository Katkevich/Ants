#pragma once
#include "Location.h"
#include "Movement.h"
#include "Ant.h"
#include "Default.h"
#include <math.h>
#include <vector>
#include <fstream>
//#include <ifstream>
#include <string>
#include <iostream>
using namespace std;

const char CDIRECTION[4] = {'N', 'E', 'S', 'W'};
const int IDIRECTION[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

class Bot
{
public:
	vector<Location> *enemyAnts, *myHills, *enemyHills, *water, *food;
	vector<Ant> *myAnts;
	vector<double> *scores;
	int rows, cols, turn, turns, players;
	double loadtime, turntime, viewradius, attackradius, spawnradius;
	bool gameover;
	long seed;

	void ParseInitialInput(istream& in);
	bool ParseInput(istream& in);	
	void Run();
	void SetTargets();
	void SearchAnt(Location target);

	void MakeMove();
	void EndMove();
	bool Parse();
	
	Bot();
	~Bot();
};

