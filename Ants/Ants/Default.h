#pragma once

enum Target: int
{
	ENEMY_ANTS,
	ENEMY_HILL,
	FOOD,
	EXPLORE,
	NOTHING
};

const char DIRECTIONS[] = { 'F', 'N', 'E', 'W', 'S' };
const float ATTACK_RADIUS_COEFF = 1.5;
const float FIND_NEAREST_ANT_COEFF = 3.0;
const float FIND_FIGHTING_ANTS_COEFF = 2.0;

#define TIMEOUT 0.6
#define TARGET_RADIUS 2
#define EXPLORE_RADIUS 20
#define MAX_DIST_TO_FOOD 15
#define DIST_TO_TARGET 50
#define RES_SQUARE 4
#define TURNS_BEFORE_DISTRIB 5
#define SAME_TARGET_ANTS 3
#define RADIUS 4
#define ENEMY_HILL_DISTANCE 15
#define FIGTHING_GROUP_COUNT 6