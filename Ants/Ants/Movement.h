#pragma once

#include "Ant.h"

static class Movement
{
public:
	char AStarForAnt(int row, int col, int maxRow, int maxCol, Ant* ant);
	Movement(void);
	~Movement(void);
};

