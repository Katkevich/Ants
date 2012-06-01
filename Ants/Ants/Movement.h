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

/**
 *	Responsible for finding ways of movement.
 */
static class M
{
public:
	/**
	 *	The number of rows the playing field.
	 */
	static int maxRows;

	/**
	 *	The number of columns the playing field.
	 */
	static int maxCols;		

	/**
	 *	Search path foe ant.
	 *  @param ant Ant, for which the path is searched.
	 *  @param bot A pointer to the bot.
	 *  @return The direction of where to go ant.
	 */
	static char SearchPathForAnt(Ant* ant, Bot* bot);

	/**
	 *	Take the Euclidean distance between two points.
	 *  @param row1 The row number of point 1.
	 *  @param col1 The column number of point 1.
	 *  @param row2 The row number of point 2.
	 *  @param col2 The column number of point 2.
	 *  @return Distance.
	 */
	static float GetDirectDistance(int row1, int col1, int row2, int col2);

	/**
	 *	Take the Manhattan distance between two points.
	 *  @param row1 The row number of point 1.
	 *  @param col1 The column number of point 1.
	 *  @param row2 The row number of point 2.
	 *  @param col2 The column number of point 2.
	 *  @return Distance.	
	 */
	static int GetDistance(int row1, int col1, int row2, int col2);

	/**
	 *	Take nearest 8 neighboring cells.
	 *  @param cell The cell, which sought to neighboring cells.
	 *  @param neighborCells Neighbor cells. (out parameter)
	 *  @param water Cells with water.
	 */
	static void GetNeighborCells(Cell* cell, vector<Cell>* neighborCells, vector<Location>* water);

	/**
	 *	Add cell to neighboring cells.
	 *  @param row The row of cell, which sought to neighboring cells.
	 *  @param col The column of cell, which sought to neighboring cells.
	 *  @param neighborCells Neighbor cells. (out parameter)
	 *  @param water Cells with water.
	 */
	static void AddNodeToNeighborCells(int row, int col, vector<Cell>* neighborCells, vector<Location>* water);

	/**
	 *	Stop the search path, if the goal is achieved.
	 *  @param currentCell Extreme point of the found path.
	 *  @param ant Ant, for which the path is searched.
	 *  @return Search is over.
	 */
	static bool SearchPathIsComplete(Cell *currentCell, Ant* ant);
	
	/**
	 *	Find an ant for purpose.
	 *  @param targetRow The row number of target cell.
	 *  @param targetCol The column number of target cell.
	 *  @param bot A pointer to the bot.
	 *  @param target Type of target cells.
	 */
	static void SearchAntForTarget(int targetRow, int targetCol, Bot* bot, Target target);

	/**
	 *	Stop the search ant, if the goal is achieved.
	 *  @param targetRow The row number of target cell.
	 *  @param targetCol The column number of target cell.
	 *  @param currentCell Extreme point of the found path.
	 *  @param bot A pointer to the bot.
	 */
	static bool SearchAntIsComplete(int targetRow, int targetCol, Cell * currentCell, Bot* bot);
	
	/**
	 *	Determine the direction of the target coordinates of the cell.
	 *  @param row1 The row number of the current position.
	 *  @param col1 The column number of the current position.
	 *  @param row2 The row number of the target cell.
	 *  @param col2 The column number of the target cell.
	 *  @return The direction of movement.
	 */
	static char GetDirection(int row1, int col1, int row2, int col2);

	/**
	 *	Determine the coordinates of the target cell in the direction of motion.
	 *  @param direction The direction of movement.
	 *  @param row The row number of the current position.
	 *  @param col The column number of the current position.
	 *  @return Coordinates of the target cell.
	 */
	static Location GetCoordOfDirection(char direction, int row, int col);

	/**
	 *	Identification of possible motion in that direction.
	 *  @param direction The direction of movement.
	 *  @param row The row number of the current position.
	 *  @param col The column number of the current position.
	 *  @param bot A pointer to the bot.
	 *  @return Is it possible to move.
	 */
	static bool CanMove(char direction, int row, int col, Bot* bot);	
};

