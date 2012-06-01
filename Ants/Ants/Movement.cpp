#include "StdAfx.h"
#include "Movement.h"

int M::maxRows = 0;
int M::maxCols = 0;

char M::SearchPathForAnt(Ant* ant, Bot* bot)
{			
	vector<Cell> closedCells, openCells, neighborCells;
	Cell start;	
	start.col = ant->location.col;
	start.row = ant->location.row;
	start.gScore = 0;
	start.hScore = GetDirectDistance(ant->location.row, ant->location.col, ant->destination.row, ant->destination.col);
	start.fScore = start.gScore + start.hScore;
	openCells.push_back(start);	
	while(!openCells.empty())
	{
		vector<Cell>::iterator cellIt = openCells.begin();
		for(auto it = openCells.begin(); it != openCells.end(); it++)
			if ((*it).fScore <= cellIt->fScore)
				cellIt = it;
		Cell *cell = new Cell((*cellIt).row, (*cellIt).col, (*cellIt).gScore,(*cellIt).hScore, 
							  (*cellIt).fScore, (*cellIt).parent);		
		if (SearchPathIsComplete(cell, ant) || cell->gScore > DIST_TO_TARGET)
		{
			Cell *currentCell = cell;	
			if(cell->gScore > DIST_TO_TARGET)
			{
				ant->destination.row = cell->row;
				ant->destination.col = cell->col;
			}
			if(currentCell->parent)
			while(currentCell->parent->parent != NULL)
			{
				currentCell = currentCell->parent;
			}				
			ant->direction = GetDirection(currentCell->row, currentCell->col, ant->location.row, ant->location.col);
			return ant->direction;			
		}
		closedCells.push_back(*cell);
		openCells.erase(cellIt);
		GetNeighborCells(cell, &neighborCells, bot->water);		
		for (auto it = neighborCells.begin(); it != neighborCells.end(); it++)
		{
			if (find(closedCells.begin(), closedCells.end(), *it) != closedCells.end())
				continue;
			int neighborGScore = cell->gScore + GetDistance(cell->row, cell->col, (*it).row, (*it).col);
			if(find(openCells.begin(), openCells.end(), *it) == openCells.end())
			{
				int gScore = neighborGScore;
				float hScore = (*it).hScore = 
					GetDirectDistance((*it).row, (*it).col, ant->destination.row, ant->destination.col);
				float fScore = neighborGScore + hScore;
				openCells.push_back(Cell((*it).row, (*it).col, gScore, hScore, fScore, cell));
			}	
		}		
	}
}

float M::GetDirectDistance(int row1, int col1, int row2, int col2)
{	
	float colDist = min(col2 + (maxCols - col1), min(abs(col1 - col2), col1 + (maxCols - col2)));
	float rowDist = min(row2 + (maxRows - row1), min(abs(row1 - row2), row1 + (maxRows - row2)));
	return sqrt(pow(colDist, 2) + pow(rowDist, 2));		 
}

int M::GetDistance(int row1, int col1, int row2, int col2)
{	
	return 
		min(row2 + (maxRows - row1), min(abs(row1 - row2), row1 + (maxRows - row2))) + 
		min(col2 + (maxCols - col1), min(abs(col1 - col2), col1 + (maxCols - col2)));
}

void M::GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water)
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

void M::AddNodeToNeighborCells(int row, int col,vector<Cell>* neighborCells, vector<Location>* water)
{
	if (find_if(water->begin(), water->end(), 
		[row, col](Location x){ return x.row == row && x.col == col; }) == water->end())
	{
		neighborCells->push_back(Cell(row, col));
	}
}

bool M::SearchPathIsComplete(Cell *x, Ant* ant)
{
	if (ant->target == EXPLORE && 
		x->row <= ant->destination.row + TARGET_RADIUS &&
		x->row >= ant->destination.row - TARGET_RADIUS &&
		x->col <= ant->destination.col + TARGET_RADIUS &&
		x->col >= ant->destination.col - TARGET_RADIUS)
	{
		return true;
	}
	else if(ant->target == FOOD &&
		ant->destination.row == x->row && ant->destination.col == x->col)
	{
		return true;
	}
	else if(ant->target == ENEMY_HILL &&
		ant->destination.row == x->row && ant->destination.col == x->col)
	{
		return true;
	}
	return false;
}

bool M::SearchAntIsComplete(int tRow, int tCol, Cell *x, Bot* bot)
{
	vector<Ant>::iterator antIt;
	if((antIt = find_if(bot->myAnts->begin(), bot->myAnts->end(), 
		[x](Ant ant){ return x->row == ant.location.row && x->col == ant.location.col; })) != bot->myAnts->end())
	{
		if(antIt->distToTarget > x->gScore && antIt->target >= FOOD)
		{
			antIt->target = FOOD;
			antIt->destination.row = tRow;
			antIt->destination.col = tCol;
			antIt->distToTarget = x->gScore;
			if (x->parent)
			{
				antIt->direction = 
					GetDirection(x->parent->row, x->parent->col, antIt->location.row, antIt->location.col);
			}
		}
		return true;
	}
	return false;
}

void M::SearchAntForTarget(int targetRow, int targetCol, Bot* bot, Target target)
{
	vector<Cell> closedCells, openCells, neighborCells;
	Cell start;
	start.col = targetCol;
	start.row = targetRow;
	start.gScore = 0;
	openCells.push_back(start);	
	int steps = 0;
	auto ito = openCells.begin();
	for(int i = 0; ; i++)
	{
		if(openCells[i].gScore > MAX_DIST_TO_FOOD)
			break;
		Cell *cell = new Cell(openCells[i].row, openCells[i].col, openCells[i].gScore, 0, 0, openCells[i].parent);
		if (SearchAntIsComplete(targetRow, targetCol, cell, bot))
		{	
			return;
		}
		GetNeighborCells(cell, &neighborCells, bot->water);		
		for (auto itn = neighborCells.begin(); itn != neighborCells.end(); itn++)
		{
			if (find(openCells.begin(), openCells.end(), *itn) != openCells.end())
			{
				continue;
			}
			openCells.push_back(Cell(itn->row, itn->col, cell->gScore + 1, 0, 0, cell));
		}	
			steps++;
	}
	return;
}

char M::GetDirection(int row1, int col1, int row2, int col2)
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

Location M::GetCoordOfDirection(char direction, int row, int col)
{
	switch(direction)
	{
	case 'N':
		row == 0 ? row = maxRows - 1 : row = row - 1;
		break;
	case 'W':
		col == 0 ? col = maxCols - 1 : col = col - 1;
		break;
	case 'S':
		row == maxRows - 1 ? row = 0 : row = row + 1;
		break;
	case 'E':
		col == maxCols - 1 ? col = 0 : col = col + 1;
		break;
	}
	return Location(row, col);
}

bool M::CanMove(char direction, int row, int col, Bot* bot)
{
	Location loc = M::GetCoordOfDirection(direction, row, col);
	return
		find_if(bot->water->begin(), bot->water->end(), 
			[&](Location _loc){ return _loc.row == loc.row && _loc.col == loc.col; }) == bot->water->end() &&
		find_if(bot->myAnts->begin(), bot->myAnts->end(), 
			[&](Ant _ant){ return _ant.location.row == loc.row && _ant.location.col == loc.col; }) == 
			bot->myAnts->end();
}
