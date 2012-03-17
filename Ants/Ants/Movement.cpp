#include "StdAfx.h"
#include "Movement.h"

char Movement::AStarForAnt(int row, int col, int maxRow, int maxCol, Ant* ant)
{	
	//ant->path->clear();
	//if(!ant->path->empty())
	//{
	//	//ant->location temp = ant->ant->path->back();
	//	//ant->ant->path->pop_back();
	//	//return temp;
	//	ant->location.col = ant->path->back().col;
	//	ant->location.row = ant->path->back().row;
	//	ant->path->pop_back();
	//	
	//	if(ant->location.row < ant->path->back().row || (ant->path->back().row == MAX_ROWS - 1 && ant->location.row == 0))
	//		return 'S';
	//	else if(ant->location.row == ant->path->back().row)
	//	{
	//		if(ant->location.col > ant->path->back().col || (ant->location.col == 0 && ant->path->back().col == MAX_COLS - 1))
	//			return 'W';
	//		else
	//			return 'E';
	//	}
	//	else
	//		return 'N';
	//}
	//MAX_ROWS = rows;
	//MAX_COLS = cols;
	//vector<Cell> closedCells, openCells, neighborCells;
	////openCells = new vector<Cell>;
	////vector<Cell> *neighborCells = new vector<Cell>;
	////vector<ant->location> *ant->path = new vector<ant->location>;
	//Cell start;
	//start.col = ant->location.col;
	//start.row = ant->location.row;
	////start.parent = &start;
	//start.gScore = 0;
	//start.hScore = GetDirectDest(ant->location.row, ant->location.col, destination.row, destination.col);
	//start.fScore = start.gScore + start.hScore;
	//openCells.push_back(start);	
	//while(!openCells.empty())
	//{
	//	vector<Cell>::iterator cellIt = openCells.begin();
	//	//vector<Cell>::iterator del = openCells.begin();
	//	// = *find_if(openCells.begin(), openCells.end(), [](Cell c){ c.distToFinish + c.distToStart =  });
	//	for(auto it = openCells.begin(); it != openCells.end(); it++)
	//		if ((*it).fScore <= cellIt->fScore)
	//			cellIt = it;
	//	Cell *cell = new Cell((*cellIt).row, (*cellIt).col, (*cellIt).gScore, (*cellIt).hScore, (*cellIt).fScore, (*cellIt).parent);
	//	if (IsFinish(cell))
	//	{
	//		Cell *currentCell = cell;
	//		while(currentCell->parent != NULL)
	//		{
	//			ant->path->push_back(ant->location(currentCell->row, currentCell->col));
	//			currentCell = currentCell->parent;
	//		}				
	//		//delete neighborCells;
	//		//ant->path->pop_back();
	//		//ant->location.row = currentCell->row;
	//		//ant->location.col = currentCell->col;
	//		//return ant->location(currentCell->row, currentCell->col);	
	//		if(ant->path->size() == 1)			
	//			this->onTheWay = false;
	//		if(ant->path->back().row == MAX_ROWS - 1 && ant->location.row == 0) return 'N';
	//		if(ant->path->back().col == 0 && ant->location.col == MAX_COLS - 1) return 'E';
	//		if(ant->path->back().col == MAX_COLS - 1 && ant->location.col == 0) return 'W';
	//		if(ant->path->back().row == 0 && ant->location.row == MAX_ROWS - 1) return 'S';
	//		
	//		if(ant->path->back().row < ant->location.row) return 'N';
	//		if(ant->location.col < ant->path->back().col) return 'E';
	//		if(ant->location.col > ant->path->back().col) return 'W';
	//		if(ant->path->back().row > ant->location.row) return 'S';			
	//		
	//	}
	//	//remove_if(openCells.begin(), openCells.end(), [cell](Cell c){ return c.col == cell->col && c.row == cell->row; });
	//	//openCells.erase(remove(openCells.begin(), openCells.end(), *cell), openCells.end());		
	//	closedCells.push_back(*cell);
	//	openCells.erase(cellIt);
	//	//remove(openCells.begin(), openCells.end(), *cell);		
	//	GetNeighborCells(cell, &neighborCells, water);
	//	for (auto it = neighborCells.begin(); it != neighborCells.end(); it++)
	//	{
	//		if (find(closedCells.begin(), closedCells.end(), *it) != closedCells.end())
	//			continue;
	//		int neighborGScore = cell->gScore + GetDest(cell->row, cell->col, (*it).row, (*it).col);
	//		//bool gScoreIsBetter = false;
	//		/*if(find(openCells.begin(), openCells.end(), *it) == openCells.end())			
	//		{
	//			openCells.push_back(*it);
	//			gScoreIsBetter = true;
	//		}
	//		else if(neighborGScore < (*it).gScore)
	//			gScoreIsBetter = true;
	//		else
	//			gScoreIsBetter = false;			
	//		if(gScoreIsBetter)
	//		{
	//			(*it).gScore = neighborGScore;
	//			int temp = (*it).hScore = GetDest((*it).row, (*it).col, destination.row, destination.col);
	//			(*it).fScore = neighborGScore + temp;
	//			(*it).parent = cell;
	//		}*/
	//		auto tempIt = find(openCells.begin(), openCells.end(), *it);
	//		
	//		if(tempIt == openCells.end())			
	//		{
	//			int gScore = neighborGScore;
	//			float hScore = (*it).hScore = GetDirectDest((*it).row, (*it).col, destination.row, destination.col);
	//			float fScore = neighborGScore + hScore;
	//			//(*it).parent = cell;
	//			//Cell *cel = new Cell((*it).row, (*it).col, gScore, hScore, fScore, cell);
	//			openCells.push_back(Cell((*it).row, (*it).col, gScore, hScore, fScore, cell));
	//			//gScoreIsBetter = true;
	//		}
	//		else if(neighborGScore < (*it).gScore)
	//		{
	//			(*tempIt).gScore = neighborGScore;
	//			float temp = (*tempIt).hScore = GetDirectDest((*tempIt).row, (*tempIt).col, destination.row, destination.col);
	//			(*tempIt).fScore = neighborGScore + temp;
	//			(*tempIt).parent = &(*cell);
	//			//gScoreIsBetter = true;
	//		}
	//		/*else
	//			gScoreIsBetter = false;	*/		
	//		/*if(gScoreIsBetter)
	//		{
	//			(*it).gScore = neighborGScore;
	//			int temp = (*it).hScore = GetDest((*it).row, (*it).col, destination.row, destination.col);
	//			(*it).fScore = neighborGScore + temp;
	//			(*it).parent = cell;
	//		}*/
	//		//delete cell;
	//	}		
	//}
	////return NULL;
}


Movement::Movement(void)
{
}


Movement::~Movement(void)
{
}
