#pragma once
#include "Location.h"
#include <ctime>
#include "Ant.h"
#include "Default.h"
#include <math.h>
#include <numeric>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

/**
 *	The cell playing field for research.
 *  All the playing field is divided into cells, which are characterized by the last visit.
 */
struct SpaceForResearch 
{
	/**
	 *	The row number of the cell.
	 */
	int row;

	/**
	 *	The column number of the cell.
	 */
	int col;

	/**
	 *	The number of moves from the last visit of the cell.
	 */
	int latestRsch;

	/**
	 *	The number of ants, which are sent to the cell.
	 */
	int antsNumb;

	/**
	 *	The distance from the cell to the current ant.
	 */
	float dist;

	/**
	 *	Constructor.	 
	 */
	SpaceForResearch()
	{
		row = col = antsNumb = 0;
		latestRsch = -1;
	}

	/**
	 *	Constructor.
	 *  Initialize the initial state.
	 *  @param row Row number.
	 *  @param col Column number.
	 *  @param latestRsch The number of moves until the last visit.
	 */
	SpaceForResearch(int row, int col, int latestRsch)
	{
		this->row = row;
		this->col = col;
		this->latestRsch = latestRsch;
		antsNumb = 0;
	}	
};

/**
 *	Responsible for the state of the game.
 */
class Bot
{
public:
	/**
	 *	A collection of enemy ants.
	 */
	vector<Location> *enemyAnts;

	/**
	 *	A collection of my hills.
	 */
	vector<Location> *myHills;

	/**
	 *	A collection of enemy hills.
	 */
	vector<Location> *enemyHills;

	/**
	 *	A collection of cells with water.
	 */
	vector<Location> *water;

	/**
	 *	A collection of cells with food.
	 */
	vector<Location> *food;

	/**
	 *	A collection of my ants.
	 */
	vector<Ant> *myAnts;	

	/**
	 *	A collection of cells for research.
	 */
	vector<SpaceForResearch> *rschSpaces;

	/**
	 *	The number of rows the playing field.
	 */
	int rows;

	/**
	 *	The number of columns the playing field.
	 */
	int cols;

	/**
	 *	Turn number.
	 */
	int turn;

	/**
	 *	The maximum number of strokes per round.
	 */
	int turns;

	/**
	 *	Time to initialize the bot.
	 */
	double loadtime;

	/**
	 *	Time to move.
	 */
	double turntime;

	/**
	 *	The radius of the review an ant.
	 */
	double viewradius;

	/**
	 *	The radius of the attack an ant.
	 */
	double attackradius;

	/**
	 *	The initial value for the random number generator.
	 */
	long randomSeed;

	/**
	 *	Find closest to the enemy ants ant to battle.
	 *  @return List of fighting ants.
	 */
	vector<pair<int, Ant>>* FindFightingAnts(void);	
	
	/**
	 *	Find the nearest enemy ants.
	 *  @param ant Ant, for which the search is conducted of enemy ants.
	 *  @param enemiesGroup Ants that are already added into the enemy group.
	 *  @return The enemy ants, which have not yet been added into the enemy group.
	 */
	vector<Ant>* GetEnemies(Ant& ant, vector<Ant>* enemiesGroup);	 

	/**
	 *	The formation of battle groups, which includes the mine, and enemy ants.
	 *  @param fightingAnts My fighting ants.
	 *  @return Battle groups.
	 */
	vector<pair<vector<Ant>*, vector<Ant>*>>* DistributionForBattleGroups(vector<pair<int, Ant>>* fightingAnts);

	/**
	 *	Find the best configuration of my ants for each group.
	 *  @param fightingGroups Fighting Group. Each member of the enemy and my ants.
	 *  @return The best configuration of my groups of ants.
	 */
	vector<vector<Ant>*>* Attack(vector<pair<vector<Ant>*, vector<Ant>*>>* fightingGroups);

	/**
	 *	Go to the next group configuration, which will change the possible location of the ants.
	 *  @param group The group of ants, which change the configuration.
	 *  @param mutableAnt Number of ants in the group, which is a possible location change.
	 *  @param groupType Group type: 1 - my / 0 - enemy.
	 *  @return Next configuration of the group.
	 */
	vector<Ant>* GoNextGroupConfig(vector<Ant>* group, int mutableAnt, int groupType);

	/**
	 *	Change the possible location of an ant, if possible.
	 *  @param ant Ant, whose state is changing.
	 *  @param group The group of ants, to which the variable ant.
	 *  @param antType Ant class: 1 - my / 0 - enemy.
	 */
	void NextDirection(Ant& ant, vector<Ant>* group, int antType);

	/**
	 *	Calculate a function that characterizes the outcome of the battle.
	 *  @param enemies The group of enemy ants.
	 *  @param myAnts The group of my ants.
	 *  @return Minimax-function value.
	 */
	float CalculateMinimax(vector<Ant>* enemies, vector<Ant>* myAnts);

	/**
	 *	Update the state of my fighting ants in accordance with the calculated configurations of battle groups.
	 *  @param fightingGroups The best configuration of my groups of ants.
	 */
	void UpdateAntsStateAccordingToAttackingGroup(vector<vector<Ant>*>* fightingGroups);	

	/**
	 *	Analysis of the initial input data.
	 *  @param in The input stream.
	 */
	void ParseInitialInput(istream& in);

	/**
	 *	Analysis of the next input data.
	 *  @param in The input stream.
	 */
	bool ParseInput(istream& in);	

	/**
	 *	Run the bot.
	 */
	void Run();

	/**
	 *	Allocate targets for ants.
	 *  @param out Time-log.
	 *  @return Successful or not.
	 */
	bool AllocTargetsForAnts(ofstream& out);
	
	/**
	 *	Reset the targets for ant researchers.
	 */
	void TargetIsReached();

	/**
	 *	Make moves in line with the targets of the ants.
	 *  @param target Ant with this type of target runs the turn.
	 *  @param nextTurnAnts Ants, which has already made ​​a move.
	 *  @param immobilAnts Ants who can not run the course in the required direction.
	 */
	void MakeMoves(Target target, vector<Location>* nextTurnAnts, vector<Ant*>* immobilAnts);

	/**
	 *	Try make moves for immobile ants.
	 *  @param immobilAnts Ants who can not run the course in the required direction.
	 */
	void TryMakeMoves(vector<Ant*> *immobilAnts);		

	/**
	 *	Constructor.
	 */	
	Bot();

	/**
	 *	Destructor.
	 */
	~Bot();
};

