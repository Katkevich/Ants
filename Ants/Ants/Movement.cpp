#include "StdAfx.h"
#include "Movement.h"

int Movement::maxRows = 0;
int Movement::maxCols = 0;

char Movement::SearchPathForAnt(Ant* ant, Bot* bot)
{		
//	ant->path->clear();	
	vector<Cell> closedCells, openCells, neighborCells;
	Cell start;
	int i = 0;
	if(ant->location.row == 32 && ant->location.col == 82)
		i++;
	start.col = ant->location.col;
	start.row = ant->location.row;
	start.gScore = 0;
	start.hScore = GetDirectDistance(ant->location.row, ant->location.col, ant->destination.row, ant->destination.col/*, maxRows, maxCols*/);
	start.fScore = start.gScore + start.hScore;
	openCells.push_back(start);	
	while(!openCells.empty())
	{
		vector<Cell>::iterator cellIt = openCells.begin();
		for(auto it = openCells.begin(); it != openCells.end(); it++)
			if ((*it).fScore <= cellIt->fScore)
				cellIt = it;
		Cell *cell = new Cell((*cellIt).row, (*cellIt).col, (*cellIt).gScore,(*cellIt).hScore, (*cellIt).fScore, (*cellIt).parent);
		if (SearchPathIsComplete(cell, ant))
		{
			Cell *currentCell = cell;
			if(currentCell->parent)
			while(currentCell->parent->parent != NULL)
			{
//				ant->path->push_back(Location(currentCell->row, currentCell->col));
				currentCell = currentCell->parent;
			}				
//			if(ant->path->size() == 1)			
//				ant->onTheWay = false;
//			ant->direction = GetDirection(ant->path->back().row, ant->path->back().col, ant->location.row, ant->location.col);
			ant->direction = GetDirection(currentCell->row, currentCell->col, ant->location.row, ant->location.col);
			return ant->direction;
			//if(ant->path->back().row == maxRows - 1 && ant->location.row == 0) return 'N';
			//if(ant->path->back().col == 0 && ant->location.col == maxCols - 1) return 'E';
			//if(ant->path->back().col == maxCols - 1 && ant->location.col == 0) return 'W';
			//if(ant->path->back().row == 0 && ant->location.row == maxRows - 1) return 'S';
			//
			//if(ant->path->back().row < ant->location.row) return 'N';
			//if(ant->location.col < ant->path->back().col) return 'E';
			//if(ant->location.col > ant->path->back().col) return 'W';
			//if(ant->path->back().row > ant->location.row) return 'S';			
			
		}
		closedCells.push_back(*cell);
		openCells.erase(cellIt);
		//GetNeighborCells(cell, maxRows, maxCols, &neighborCells, bot->water);		
		GetNeighborCells(cell, &neighborCells, bot->water);		
		for (auto it = neighborCells.begin(); it != neighborCells.end(); it++)
		{
			if (find(closedCells.begin(), closedCells.end(), *it) != closedCells.end())
				continue;
			int neighborGScore = cell->gScore + GetDistance(cell->row, cell->col, (*it).row, (*it).col/*, maxRows, maxCols*/);
			
			//auto tempIt = find(openCells.begin(), openCells.end(), *it);
			
			//if(tempIt == openCells.end())			
			if(find(openCells.begin(), openCells.end(), *it) == openCells.end())
			{
				int gScore = neighborGScore;
				float hScore = (*it).hScore = GetDirectDistance((*it).row, (*it).col, ant->destination.row, ant->destination.col/*, maxRows, maxCols*/);
				float fScore = neighborGScore + hScore;
				openCells.push_back(Cell((*it).row, (*it).col, gScore, hScore, fScore, cell));
			}
			//else if(neighborGScore < (*it).gScore)
			//{
			//	(*tempIt).gScore = neighborGScore;
			//	float temp = (*tempIt).hScore = GetDirectDistance((*tempIt).row, (*tempIt).col, ant->destination.row, ant->destination.col/*, maxRows, maxCols*/);
			//	(*tempIt).fScore = neighborGScore + temp;
			//	(*tempIt).parent = &(*cell);
			//}			
		}		
	}
}

//float Movement::GetDirectDistance(int row1, int col1, int row2, int col2, int maxRows, int maxCols)
float Movement::GetDirectDistance(int row1, int col1, int row2, int col2)
{	
	float colDiff = min(col2 + (maxCols - col1), min(abs(col1 - col2), col1 + (maxCols - col2)));
	float rowDiff = min(row2 + (maxRows - row1), min(abs(row1 - row2), row1 + (maxRows - row2)));
	return sqrt(pow(colDiff, 2) + pow(rowDiff, 2));		 
}

//int Movement::GetDistance(int row1, int col1, int row2, int col2, int maxRows, int maxCols)
int Movement::GetDistance(int row1, int col1, int row2, int col2)
{	
	return 
		min(row2 + (maxRows - row1), min(abs(row1 - row2), row1 + (maxRows - row2))) + 
		min(col2 + (maxCols - col1), min(abs(col1 - col2), col1 + (maxCols - col2)));
}

//void Movement::GetNeighborCells(Cell* parent, int maxRows, int maxCols, vector<Cell>* neighborCells, vector<Location>* water)
void Movement::GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water)
{
	neighborCells->clear();
	int rowLess, rowMore, colLess, colMore;
	parent->row == 0 ? rowLess = maxRows - 1 : rowLess = parent->row - 1;
	parent->row == maxRows - 1 ? rowMore = 0 : rowMore = parent->row + 1;
	parent->col == 0 ? colLess = maxCols - 1 : colLess = parent->col - 1;
	parent->col == maxCols - 1 ? colMore = 0 : colMore = parent->col + 1;	
	AddNodeToNeighborCells(rowLess, parent->col, neighborCells, water);
	AddNodeToNeighborCells(parent->row, colLess, neighborCells, water);
	AddNodeToNeighborCells(parent->row, colMore, neighborCells, water);
	AddNodeToNeighborCells(rowMore, parent->col, neighborCells, water);
}

void Movement::AddNodeToNeighborCells(int row, int col,vector<Cell>* neighborCells, vector<Location>* water)
{
	if (find_if(water->begin(), water->end(), [row, col](Location x){ return x.row == row && x.col == col; }) == water->end())
		neighborCells->push_back(Cell(row, col));
}

bool Movement::SearchPathIsComplete(Cell *x, Ant* ant)
{
	if (ant->target == EXPLORE && 
		x->row <= ant->destination.row + TARGET_RADIUS &&
		x->row >= ant->destination.row - TARGET_RADIUS &&
		x->col <= ant->destination.col + TARGET_RADIUS &&
		x->col >= ant->destination.col - TARGET_RADIUS)
		return true;
	else if(ant->target == FOOD &&
		ant->destination.row == x->row && ant->destination.col == x->col)
		return true;
	return false;
}

bool Movement::SearchAntIsComplete(int tRow, int tCol, Cell *x, Bot* bot)
{
	vector<Ant>::iterator antIt;
	if((antIt = find_if(bot->myAnts->begin(), bot->myAnts->end(), 
		[x](Ant ant){ return x->row == ant.location.row && x->col == ant.location.col; })) != bot->myAnts->end())
	{
		if(antIt->distToTarget > x->gScore)
		{
			antIt->target = FOOD;
			antIt->destination.row = tRow;
			antIt->destination.col = tCol;
			antIt->onTheWay = false;
			antIt->distToTarget = x->gScore;
			if (x->parent)
				antIt->direction = GetDirection(x->parent->row, x->parent->col, antIt->location.row, antIt->location.col);
		}
		return true;
	}
	return false;
}

//char Movement::SearchAntForTarget(int maxR, int maxC, int targetRow, int targetCol, Bot* bot, Target target)
char Movement::SearchAntForTarget(int targetRow, int targetCol, Bot* bot, Target target)
{
	//maxRows = maxR;
	//maxCols = maxC;
	vector<Cell> closedCells, openCells, neighborCells;
	Cell start;
	start.col = targetCol;
	start.row = targetRow;
	start.gScore = 0;
	openCells.push_back(start);	
	int steps = 0;
	auto ito = openCells.begin();
	int *o = new int();
	for(int i = 0; ; i++)
	{
		if(openCells[i].gScore > MAX_DIST_TO_FOOD)
			break;
		Cell *cell = new Cell(openCells[i].row, openCells[i].col, openCells[i].gScore, 0, 0, openCells[i].parent);
		//Cell *cell = new Cell((*ito).row, (*ito).col, steps, 0, 0, (*ito).parent);
		(*o)++;
		if (SearchAntIsComplete(targetRow, targetCol, cell, bot))
		{
			/*Cell *currentCell = cell;
			while(currentCell->parent != NULL)
			{
				ant->path->push_back(Location(currentCell->row, currentCell->col));
				currentCell = currentCell->parent;
			}				
			if(ant->path->size() == 1)			
				ant->onTheWay = false;
			if(ant->path->back().row == maxRows - 1 && ant->location.row == 0) return 'N';
			if(ant->path->back().col == 0 && ant->location.col == maxCols - 1) return 'E';
			if(ant->path->back().col == maxCols - 1 && ant->location.col == 0) return 'W';
			if(ant->path->back().row == 0 && ant->location.row == maxRows - 1) return 'S';

			if(ant->path->back().row < ant->location.row) return 'N';
			if(ant->location.col < ant->path->back().col) return 'E';
			if(ant->location.col > ant->path->back().col) return 'W';
			if(ant->path->back().row > ant->location.row) return 'S';		*/	
			return 'W';

		}
		//closedCells.push_back(*cell);
		//openCells.erase(cellIt);
		GetNeighborCells(cell, &neighborCells, bot->water);		
		//GetNeighborCells(cell, maxRows, maxCols, &neighborCells, bot->water);		
		for (auto itn = neighborCells.begin(); itn != neighborCells.end(); itn++)
		{
			if (find(openCells.begin(), openCells.end(), *itn) != openCells.end())
				continue;
			//(*itn).gScore = i;
			//(*itn).parent = cell;
			//auto tempIt = find(openCells.begin(), openCells.end(), *it);
			openCells.push_back(Cell(itn->row, itn->col, cell->gScore + 1, 0, 0, cell));

/*
			if(tempIt == openCells.end())			
			{
				int gScore = neighborGScore;
				float hScore = (*it).hScore = GetDirectDestination((*it).row, (*it).col, ant->destination.row, ant->destination.col, maxRows, maxCols);
				float fScore = neighborGScore + hScore;
				openCells.push_back(Cell((*it).row, (*it).col, gScore, hScore, fScore, cell));
			}
			else if(neighborGScore < (*it).gScore)
			{
				(*tempIt).gScore = neighborGScore;
				float temp = (*tempIt).hScore = GetDirectDestination((*tempIt).row, (*tempIt).col, ant->destination.row, ant->destination.col, maxRows, maxCols);
				(*tempIt).fScore = neighborGScore + temp;
				(*tempIt).parent = &(*cell);
			}			*/
		}	
		//if (i == openCells.size() - 1)
			steps++;
		/*if(ito == openCells.end())
			break;*/
		//ito++;
	}
	return 0;
}

//char Movement::GetDirection(int row1, int col1, int row2, int col2, int maxRows, int maxCols)
char Movement::GetDirection(int row1, int col1, int row2, int col2)
{
	if(row1 == row2 && col2 == col1) return 'W';
	if(row1 == maxRows - 1 && row2 == 0) return 'N';
	if(col1 == 0 && col2 == maxCols - 1) return 'E';
	if(col1 == maxCols - 1 && col2 == 0) return 'W';
	if(row1 == 0 && row2 == maxRows - 1) return 'S';

	if(row1 < row2) return 'N';
	if(col2 < col1) return 'E';
	if(col2 > col1) return 'W';
	if(row1 > row2) return 'S';
}

Movement::Movement(void)
{
	//maxCols = maxRows = 0;
}


Movement::~Movement(void)
{
}
