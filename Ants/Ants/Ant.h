#pragma once
#include "Location.h"
#include "Default.h"
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
using namespace std;

/**
 *	Responsible for the state of the cell field.
 *  It is used in the algorithms of movements.
 */
struct Cell 
{	
	/**	 
	 *	The distance to the ant.
	 */
	int gScore;

	/**	 
	 *	The distance to the goal.
	 */
	float hScore;

	/**
	 *	The distance from the ant to the target.
	 *  The sum of gScore and hScore.
	 */
	float fScore;

	/**
	 *	A pointer to the parent cell.
	 */
	Cell* parent;

	/**
	 *	The row number of the cell.
	 */
	int row;

	/**
	 *	The column number of the cell.
	 */
	int col;

	/**
	 *	Constructor.
	 */
	Cell()
	{
		gScore = row = col = 0;
		hScore = fScore = 40000;
		parent = NULL;
	}

	/**
	 *	Constructor.
	 *  @param row The row number of the cell.
	 *  @param col The column number of the cell.
	 */
	Cell(int row, int col)
	{
		this->row = row;
		this->col = col;
		gScore =  0;
		hScore = fScore = 40000;
		parent = NULL;
	}

	/**
	 *	Constructor.
	 *  @param row The row number of the cell.
	 *  @param col The column number of the cell.
	 *  @param gScore The distance to the ant.
	 *  @param hScore The distance to the goal.
	 *  @param fScore The sum of gScore and hScore.
	 *  @param parent A pointer to the parent cell.
	 */
	Cell(int row, int col, float gScore, float hScore, float fScore, Cell* parent)
	{
		this->row = row;
		this->col = col;
		this->gScore = gScore;
		this->hScore = hScore;
		this->fScore = fScore;
		this->parent = parent;
	}

	/**
	 *	Copy constructor.
	 *  @param cell Copied cell.
	 */
	Cell(const Cell& cell)
	{
		this->parent = cell.parent;
		this->gScore = cell.gScore;
		this->hScore = cell.hScore;
		this->fScore = cell.fScore;
		this->row = cell.row;
		this->col = cell.col;
	}

	/**
	 *	Comparison operator.
	 *  Comparison is made to the coordinates.
	 *  @param cell Compare the cell.
	 */
	bool operator==(Cell cell)
	{
		return 
			cell.col == col && 
			cell.row == row;
	}

	/**
	 *	Comparison operator.
	 *  Comparison is made to the coordinates.
	 *  @param cell Compare the cell.
	 */
	bool operator!=(Cell cell)
	{
		return 
			cell.col != col ||
			cell.row != row;
	}
};

/**
 *	Responsible for the state and the movement of an ant.
 */
class Ant
{
public:
	/**
	 *	Ant location.
	 */
	Location location;

	/**
	 *	Ant destination.
	 */
	Location destination;

	/**
	 *	The distance to the target.
	 */
	int distToTarget;

	/**
	 *	Number of turn.	 
	 */
	int turn;

	/**
	 *	The direction of movement of an ant.
	 *  'N' - North, 'E' - East, 'W' - West, 'S' - South.
	 */
	char direction;

	/**
	 *	Type ant targets.
	 */
	Target target;	
		
	/**
	 *	Running ant move.
	 *  Changing the state of the ant under the direction of motion.
	 *  @param direction The direction of travel.
	 *  @param rows The size of the playing field vertically.
	 *  @param cols The size of the playing field horizontally.
	 */
	void ChangeLocation(char direction, int rows, int cols);	

	/**
	 *	Constructor.
	 *  Create a new ant with coordinates [row, col]
	 */
	Ant(int row, int col);
};