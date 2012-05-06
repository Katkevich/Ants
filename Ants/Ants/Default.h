#pragma once

enum Target: int
{
	ENEMY_HILL,
	FOOD,
	MY_HILL,
	MY_ANTS,
	ENEMY_ANTS,
	EXPLORE,
	NOTHING
};

const char DIRECTION[] = {'N', 'E', 'W', 'S'};

#define TARGET_RADIUS 2
#define EXPLORE_RADIUS 20
#define MAX_DIST_TO_FOOD 15
#define DIST_TO_TARGET 15
//#define SIZE_OF_EXP_PLACES 10
#define RES_SQUARE 4
#define TURNS_BEFORE_DISTRIB 5
#define SAME_TARGET_ANTS 3
#define TURNS_IN_EXPLORE 30
#define RADIUS 4
#define ENEMY_HILL_DISTANCE 15