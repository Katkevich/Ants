#include "StdAfx.h"
#include "Ant.h"

int MAX_ROWS, MAX_COLS;

//void AddNodeToNeighborCells(int row, int col,vector<Cell>* neighborCells, vector<Location>* water);

Ant::Ant(void)
{
	//path = new vector<Location>();
}

Ant::Ant(int row, int col)
{
	destination.row = -1;
	destination.col = -1;
	location.row = row;
	location.col = col;
	turn = 0;
	distToTarget = 40000;
	//path = new vector<Location>();
	target = NOTHING;
}

Ant::~Ant(void)
{	
	//if(path) delete path;
}
//
//char Ant::MakeMove(int rows, int cols, vector<Location>* water)
//{	
//	path->clear();
//	if(!path->empty())
//	{
//		//Location temp = path->back();
//		//path->pop_back();
//		//return temp;
//		location.col = path->back().col;
//		location.row = path->back().row;
//		path->pop_back();
//		
//		if(location.row < path->back().row || (path->back().row == MAX_ROWS - 1 && location.row == 0))
//			return 'S';
//		else if(location.row == path->back().row)
//		{
//			if(location.col > path->back().col || (location.col == 0 && path->back().col == MAX_COLS - 1))
//				return 'W';
//			else
//				return 'E';
//		}
//		else
//			return 'N';
//	}
//	MAX_ROWS = rows;
//	MAX_COLS = cols;
//	vector<Cell> closedCells, openCells, neighborCells;
//	//openCells = new vector<Cell>;
//	//vector<Cell> *neighborCells = new vector<Cell>;
//	//vector<Location> *path = new vector<Location>;
//	Cell start;
//	start.col = location.col;
//	start.row = location.row;
//	//start.parent = &start;
//	start.gScore = 0;
//	start.hScore = GetDirectDest(location.row, location.col, destination.row, destination.col);
//	start.fScore = start.gScore + start.hScore;
//	openCells.push_back(start);	
//	while(!openCells.empty())
//	{
//		vector<Cell>::iterator cellIt = openCells.begin();
//		//vector<Cell>::iterator del = openCells.begin();
//		// = *find_if(openCells.begin(), openCells.end(), [](Cell c){ c.distToFinish + c.distToStart =  });
//		for(auto it = openCells.begin(); it != openCells.end(); it++)
//			if ((*it).fScore < cellIt->fScore)/* || 
//				((*it).fScore == cellIt->fScore && (*it).dirGScore == cellIt->dirGScore))*/
//				cellIt = it;			
//		Cell *cell = new Cell((*cellIt).row, (*cellIt).col, (*cellIt).gScore, /*(*cellIt).dirGScore, */(*cellIt).hScore, (*cellIt).fScore, (*cellIt).parent);
//		if (IsFinish(cell))
//		{
//			Cell *currentCell = cell;
//			while(currentCell->parent != NULL)
//			{
//				path->push_back(Location(currentCell->row, currentCell->col));
//				currentCell = currentCell->parent;
//			}				
//			//delete neighborCells;
//			//path->pop_back();
//			//location.row = currentCell->row;
//			//location.col = currentCell->col;
//			//return Location(currentCell->row, currentCell->col);	
//			if(path->size() == 1)			
//				this->onTheWay = false;
//			if(path->back().row == MAX_ROWS - 1 && location.row == 0) return 'N';
//			if(path->back().col == 0 && location.col == MAX_COLS - 1) return 'E';
//			if(path->back().col == MAX_COLS - 1 && location.col == 0) return 'W';
//			if(path->back().row == 0 && location.row == MAX_ROWS - 1) return 'S';
//			
//			if(path->back().row < location.row) return 'N';
//			if(location.col < path->back().col) return 'E';
//			if(location.col > path->back().col) return 'W';
//			if(path->back().row > location.row) return 'S';			
//			
//		}
//		//remove_if(openCells.begin(), openCells.end(), [cell](Cell c){ return c.col == cell->col && c.row == cell->row; });
//		//openCells.erase(remove(openCells.begin(), openCells.end(), *cell), openCells.end());		
//		closedCells.push_back(*cell);
//		openCells.erase(cellIt);
//		//remove(openCells.begin(), openCells.end(), *cell);		
//		GetNeighborCells(cell, &neighborCells, water);
//		for (auto it = neighborCells.begin(); it != neighborCells.end(); it++)
//		{
//			if (find(closedCells.begin(), closedCells.end(), *it) != closedCells.end())
//				continue;
//			float neighborGScore = cell->gScore + GetDest(cell->row, cell->col, (*it).row, (*it).col);//int
//			//bool gScoreIsBetter = false;
//			/*if(find(openCells.begin(), openCells.end(), *it) == openCells.end())			
//			{
//				openCells.push_back(*it);
//				gScoreIsBetter = true;
//			}
//			else if(neighborGScore < (*it).gScore)
//				gScoreIsBetter = true;
//			else
//				gScoreIsBetter = false;			
//			if(gScoreIsBetter)
//			{
//				(*it).gScore = neighborGScore;
//				int temp = (*it).hScore = GetDest((*it).row, (*it).col, destination.row, destination.col);
//				(*it).fScore = neighborGScore + temp;
//				(*it).parent = cell;
//			}*/
//			auto tempIt = find(openCells.begin(), openCells.end(), *it);
//			
//			if(tempIt == openCells.end())			
//			{
//				float gScore = neighborGScore;//int
//				float hScore = (*it).hScore = GetDirectDest((*it).row, (*it).col, destination.row, destination.col);
//				float fScore = neighborGScore + hScore;
//				//float dirGScore = GetDirectDest((*it).row, (*it).col, location.row, location.col);
//				//(*it).parent = cell;
//				//Cell *cel = new Cell((*it).row, (*it).col, gScore, hScore, fScore, cell);
//				openCells.push_back(Cell((*it).row, (*it).col, gScore, /*dirGScore, */hScore, fScore, cell));
//				//gScoreIsBetter = true;
//			}
//			else if(neighborGScore < (*it).gScore)
//			{
//				(*tempIt).gScore = neighborGScore;
//				float temp = (*tempIt).hScore = GetDirectDest((*tempIt).row, (*tempIt).col, destination.row, destination.col);
//				(*tempIt).fScore = neighborGScore + temp;
//				//(*tempIt).dirGScore = GetDirectDest((*tempIt).row, (*tempIt).col, location.row, location.col);
//				(*tempIt).parent = &(*cell);
//				//gScoreIsBetter = true;
//			}
//			/*else
//				gScoreIsBetter = false;	*/		
//			/*if(gScoreIsBetter)
//			{
//				(*it).gScore = neighborGScore;
//				int temp = (*it).hScore = GetDest((*it).row, (*it).col, destination.row, destination.col);
//				(*it).fScore = neighborGScore + temp;
//				(*it).parent = cell;
//			}*/
//			//delete cell;
//		}		
//	}
//	//return NULL;
//}

void Ant::ChangeLocation(char direction, int maxRows, int maxCols)
{
	/*switch(direction)
	{
	case 'N':
		location.row == 0 ? location.row = MAX_ROWS - 1 : location.row = location.row - 1;
		break;
	case 'W':
		location.col == 0 ? location.col = MAX_COLS - 1 : location.col = location.col - 1;
		break;
	case 'S':
		location.row == MAX_ROWS - 1 ? location.row = 0 : location.row = location.row + 1;
		break;
	case 'E':
		location.col == MAX_COLS - 1 ? location.col = 0 : location.col = location.col + 1;
		break;
	}*/
	switch(direction)
	{
	case 'N':
		location.row == 0 ? location.row = maxRows - 1 : location.row = location.row - 1;
		break;
	case 'W':
		location.col == 0 ? location.col = maxCols - 1 : location.col = location.col - 1;
		break;
	case 'S':
		location.row == maxRows - 1 ? location.row = 0 : location.row = location.row + 1;
		break;
	case 'E':
		location.col == maxCols - 1 ? location.col = 0 : location.col = location.col + 1;
		break;
	}
}

//void Ant::GetNeighborCells(Cell* parent, vector<Cell>* neighborCells, vector<Location>* water)
//{
//	neighborCells->clear();
//	int rowLess, rowMore, colLess, colMore;
//	parent->row == 0 ? rowLess = MAX_ROWS - 1 : rowLess = parent->row - 1;
//	parent->row == MAX_ROWS - 1 ? rowMore = 0 : rowMore = parent->row + 1;
//	parent->col == 0 ? colLess = MAX_COLS - 1 : colLess = parent->col - 1;
//	parent->col == MAX_COLS - 1 ? colMore = 0 : colMore = parent->col + 1;
//	//if (find_if(water->begin(), water->end(), [rowLess, colLess](Location x){ return x.row == rowLess && x.col == colLess; }) == water->end())
//	//	neighborCells->push_back(Cell(rowLess, colLess));
//	//neighborCells->push_back(Cell(rowLess, parent->col));
//	//neighborCells->push_back(Cell(rowLess, colMore));
//	//neighborCells->push_back(Cell(parent->row, colLess));
//	//neighborCells->push_back(Cell(parent->col, colMore));
//	//neighborCells->push_back(Cell(rowMore, colLess));
//	//neighborCells->push_back(Cell(rowMore, parent->col));
//	//neighborCells->push_back(Cell(rowMore, colMore));
//	//AddNodeToNeighborCells(rowLess, colLess, neighborCells, water);
//	AddNodeToNeighborCells(rowLess, parent->col, neighborCells, water);
//	//AddNodeToNeighborCells(rowLess, colMore, neighborCells, water);
//	AddNodeToNeighborCells(parent->row, colLess, neighborCells, water);
//	AddNodeToNeighborCells(parent->row, colMore, neighborCells, water);
//	//AddNodeToNeighborCells(rowMore, colLess, neighborCells, water);
//	AddNodeToNeighborCells(rowMore, parent->col, neighborCells, water);
//	//AddNodeToNeighborCells(rowMore, colMore, neighborCells, water);
//}
//
//void AddNodeToNeighborCells(int row, int col,vector<Cell>* neighborCells, vector<Location>* water)
//{
//	if (find_if(water->begin(), water->end(), [row, col](Location x){ return x.row == row && x.col == col; }) == water->end())
//		neighborCells->push_back(Cell(row, col));
//}
//
//bool Ant::IsFinish(Cell *x)
//{
//	int t = TARGET_RADIUS;
//	if (this->target == EXPLORE)// &&
//		if(x->row <= destination.row + t)// &&
//		if(x->row >= destination.row - t)// &&
//		if(x->col <= destination.col + t)// &&
//		if(x->col >= destination.col - t)
//		return true;
//	else if(this->target == FOOD &&
//		destination.row == x->row && destination.col == x->col)
//		return true;
//	return false;
//}
//
//float Ant::GetDirectDest(int row1, int col1, int row2, int col2)
//{	
//	/*int r1 = row2 + (MAX_ROWS - row1);
//	int r2 = abs(row1 - row2);
//	int r3 = row1 + (MAX_ROWS - row2);
//	int c1 = col2 + (MAX_COLS - col1);
//	int c2 = abs(col1 - col2);
//	int c3 = col1 + (MAX_COLS - col2);*/
//	float colDiff = min(col2 + (MAX_COLS - col1), min(abs(col1 - col2), col1 + (MAX_COLS - col2)));
//	float rowDiff = min(row2 + (MAX_ROWS - row1), min(abs(row1 - row2), row1 + (MAX_ROWS - row2)));
//	return sqrt(pow(colDiff, 2) + pow(rowDiff, 2));		 
//}
//
//int Ant::GetDest(int row1, int col1, int row2, int col2)
//{	
//	/*int r1 = row2 + (MAX_ROWS - row1);
//	int r2 = abs(row1 - row2);
//	int r3 = row1 + (MAX_ROWS - row2);
//	int c1 = col2 + (MAX_COLS - col1);
//	int c2 = abs(col1 - col2);
//	int c3 = col1 + (MAX_COLS - col2);*/
//	return 
//		min(row2 + (MAX_ROWS - row1), min(abs(row1 - row2), row1 + (MAX_ROWS - row2))) + 
//		min(col2 + (MAX_COLS - col1), min(abs(col1 - col2), col1 + (MAX_COLS - col2)));
//}