#pragma once
#include "Location.h"
#include <ctime>
//#include "Movement.h"
#include "Ant.h"
#include "Default.h"
#include <math.h>
#include <numeric>
#include <vector>
#include <fstream>
//#include <ifstream>
#include <string>
#include <iostream>
using namespace std;

//const char CDIRECTION[4] = {'N', 'E', 'S', 'W'};
//const int IDIRECTION[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
struct SpaceForResearch 
{
	int row, col, latestRsch, antsNumb;
	float dist;
	SpaceForResearch()
	{
		row = col = antsNumb = 0;
		latestRsch = -1; //NEVER
	}
	SpaceForResearch(int row, int col, int latestRsch)
	{
		this->row = row;
		this->col = col;
		this->latestRsch = latestRsch;
		antsNumb = 0;
	}
	/*void Update(int latestRsch)
	{
		this->latestRsch = latestRsch;
	}*/
};

class Bot
{
public:
	vector<Location> *enemyAnts, *myHills, *enemyHills, *water, *food;
	vector<Ant> *myAnts;
	vector<double> *scores;
	vector<SpaceForResearch> *rschSpaces;
	int rows, cols, turn, turns, players;
	double loadtime, turntime, viewradius, attackradius, spawnradius;
	bool gameover;
	long seed;

	void ParseInitialInput(istream& in);
	bool ParseInput(istream& in);	
	void Run();
	void AllocTargetsForAnts();
	void SearchAnt(Location target);
	//void UpdateRschSpaces();

	void TargetIsReached();

	void MakeMoves(Target t, vector<Location>* turnsVector, vector<Ant*>* immobilAnts);
	// To try to make a move the immobilised ant
	void TryMakeMoves(vector<Ant*> *immobilAnts);
	void EndMove();
	bool Parse();
	
	Bot();
	~Bot();
};

