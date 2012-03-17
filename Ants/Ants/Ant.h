#pragma once
#include "Location.h"
#include "Default.h"
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
using namespace std;


struct Cell 
{
	Cell* parent;
	int gScore;
	float hScore, fScore;
	int row, col;
	//void GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water);
	Cell()
	{
		gScore = row = col = 0;
		hScore = fScore = 40000;
		parent = NULL;
	}
	Cell(int row, int col)
	{
		this->row = row;
		this->col = col;
		gScore = 0;
		hScore = fScore = 40000;
		parent = NULL;
	}
	Cell(int row, int col, int gScore, float hScore, float fScore, Cell* parent)
	{
		this->row = row;
		this->col = col;
		this->gScore = gScore;
		this->hScore = hScore;
		this->fScore = fScore;
		this->parent = parent;
	}
	Cell(const Cell& cell)
	{
		this->parent = cell.parent;
		this->gScore = cell.gScore;
		this->hScore = cell.hScore;
		this->fScore = cell.fScore;
		this->row = cell.row;
		this->col = cell.col;
	}
	~Cell()
	{
		/*if(this->parent)
		{
			delete parent;
			parent = NULL;
		}*/
	}
	bool operator==(Cell cell)
	{
		return 
			cell.col == col && 
			cell.row == row;
	}
	bool operator!=(Cell cell)
	{
		return 
			cell.col != col ||
			cell.row != row;
	}
};

class Ant
{
public:
	friend Cell;
	Location location;
	Location destination;
	Target target;
	bool onTheWay;
	vector<Location>* path;

	//vector<Location>* MakeMove(int MAX_ROWS, int MAX_COLS, vector<Location>* water);
	char MakeMove(int MAX_ROWS, int MAX_COLS, vector<Location>* water);
	void ChangeLocation(char direction);

	int GetDest(int row1, int col1, int row2, int col2);
	float GetDirectDest(int row1, int col1, int row2, int col2);
	bool IsFinish(Cell *x);
	void GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water);
	Ant(void);
	Ant(int row, int col);
	~Ant(void);
};