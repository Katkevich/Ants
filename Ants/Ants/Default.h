#pragma once

enum Target: int
{
	ENEMY_HILL,
	FOOD,
	MY_HILL,
	MY_ANTS,
	ENEMY_ANTS,
	EXPLORE
};

#define TARGET_RADIUS 2