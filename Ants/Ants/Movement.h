#pragma once

#include <math.h>
#include <algorithm>
#include <vector>
#include "Location.h"
#include "Bot.h"
#include "Ant.h"
#include "Default.h"
#include <iostream>
using namespace std;

static class Movement
{
public:
	static int maxRows, maxCols;	
	// A star
	static char SearchPathForAnt(Ant* ant, Bot* bot);
	static float GetDirectDistance(int row1, int col1, int row2, int col2);
	static int GetDistance(int row1, int col1, int row2, int col2);
	static void GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water);
	static void AddNodeToNeighborCells(int row, int col, vector<Cell>* neighborCells, vector<Location>* water);
	static bool SearchPathIsComplete(Cell *x, Ant* ant);
	// A star

	// Wave
	static char SearchAntForTarget(int targetRow, int targetCol, Bot* bot, Target target);
	static bool SearchAntIsComplete(int tRow, int tCol, Cell *x, Bot* bot);
	// Wave
	static char GetDirection(int row1, int col1, int row2, int col2);

	Movement(void);
	~Movement(void);
};

