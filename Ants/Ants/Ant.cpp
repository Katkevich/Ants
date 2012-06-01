#include "StdAfx.h"
#include "Ant.h"

int MAX_ROWS, MAX_COLS;

Ant::Ant(int row, int col)
{
	destination.row = -1;
	destination.col = -1;
	location.row = row;
	location.col = col;
	turn = 0;
	distToTarget = 40000;
	target = NOTHING;
	direction = 'F';
}

void Ant::ChangeLocation(char direction, int maxRows, int maxCols)
{
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