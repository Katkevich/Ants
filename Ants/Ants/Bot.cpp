#include "StdAfx.h"
#include "Bot.h"
#include "Movement.h"

bool flag = true;

Bot::Bot()
{
	enemyHills = new vector<Location>();
	enemyAnts = new vector<Location>();
	myAnts = new vector<Ant>();
	myHills = new vector<Location>();
	water = new vector<Location>();
	food = new vector<Location>();
	rschSpaces = new vector<SpaceForResearch>();
}

Bot::~Bot()
{
	delete enemyAnts; enemyAnts = NULL;
	delete enemyHills; enemyHills = NULL;
	delete myHills; myHills = NULL;
	delete myAnts; myAnts = NULL;
	delete water; water = NULL;
	delete food; food = NULL;
	delete rschSpaces; rschSpaces = NULL;
}

void Bot::Run()
{
	bool exit;	
	int row, col, player;
	ifstream in("input.txt");

	ParseInitialInput(cin);
	M::maxRows = rows;
	M::maxCols = cols;	
	for(int i = RES_SQUARE; i < rows; i += 2 * RES_SQUARE)
		for(int j = RES_SQUARE; j < cols; j += 2 * RES_SQUARE)
			rschSpaces->push_back(SpaceForResearch(i, j, 1));
	cout << "go" << endl;
	vector<Location> *nextTurnAnts = new vector<Location>();
	vector<Ant*> *immobilAnts = new vector<Ant*>();
	ofstream out("output.txt", ios::trunc);
	do
	{	
		long c;
		c = clock();
		exit = ParseInput(cin);
		for_each(rschSpaces->begin(), rschSpaces->end(), [&](SpaceForResearch &space){
			if(find_if(myAnts->begin(), myAnts->end(), [&](Ant _ant){ 
				return M::GetDirectDistance(space.row, space.col, _ant.location.row, _ant.location.col) < viewradius;
			}) != myAnts->end())
				space.latestRsch = 0;
		});				
		if(turn == 226)
		{
			cout << ' ';
		}		
		AllocTargetsForAnts(out);		
		MakeMoves(ENEMY_ANTS, nextTurnAnts, immobilAnts);
		MakeMoves(ENEMY_HILL, nextTurnAnts, immobilAnts);
		MakeMoves(FOOD, nextTurnAnts, immobilAnts);
		MakeMoves(EXPLORE, nextTurnAnts, immobilAnts);
		TryMakeMoves(immobilAnts);		
		enemyHills->erase(remove_if(enemyHills->begin(), enemyHills->end(), [&](Location hill){
			return(find_if(myAnts->begin(), myAnts->end(), [&](Ant ant) -> bool{
				return ant.location.row == hill.row && ant.location.col == hill.col;
			}) != myAnts->end());
		}), enemyHills->end());

		nextTurnAnts->clear();
		immobilAnts->clear();
		cout << "go\n";

		for_each(rschSpaces->begin(), rschSpaces->end(), [](SpaceForResearch &space){
			space.latestRsch++;
		});
		c = clock() - c;
		cout << turn << " " << (double)c/CLOCKS_PER_SEC << endl;
	} while(exit);
	delete nextTurnAnts;
	delete immobilAnts;
	in.close();
}

vector<pair<vector<Ant>*, vector<Ant>*>>* Bot::DistributionForBattleGroups(vector<pair<int, Ant>>* fightingAnts)
{
	auto pairsGroup = new vector<pair<vector<Ant>*, vector<Ant>*>>();

	int addedAnts = 0;
	int groupNumber = 0;
	while(addedAnts != fightingAnts->size())
	{
		auto myAntsGroup = new vector<Ant>();
		auto enemyAntsGroup = new vector<Ant>();
		auto ant = find_if(fightingAnts->begin(), fightingAnts->end(), [&](pair<int, Ant> _pair){
			return _pair.first == -1;
		});
		ant->first = groupNumber;
		myAntsGroup->push_back((Ant)ant->second);
		myAntsGroup->back().destination.row = ant->second.location.row;
		myAntsGroup->back().destination.col = ant->second.location.col;
		addedAnts++;
		while(1)
		{
			auto enemies = GetEnemies(myAntsGroup->back(), enemyAntsGroup);
			int enemyGroupSize = enemies->size();
			int maxTotalSize = 6 - enemyAntsGroup->size() - myAntsGroup->size();
			if(enemyGroupSize < maxTotalSize)
			{
				for_each(enemies->begin(), enemies->end(), [&](Ant _ant){
					enemyAntsGroup->push_back(Ant(_ant.location.row, _ant.location.col));
					enemyAntsGroup->back().destination.row = _ant.location.row;
					enemyAntsGroup->back().destination.col = _ant.location.col;
				});
			}
			else if(enemyGroupSize == maxTotalSize)
			{
				for_each(enemies->begin(), enemies->end(), [&](Ant _ant){
					enemyAntsGroup->push_back(Ant(_ant.location.row, _ant.location.col));
					enemyAntsGroup->back().destination.row = _ant.location.row;
					enemyAntsGroup->back().destination.col = _ant.location.col;
				});
				break;
			}
			else
			{
				int i = 0;
				for(auto it = enemies->begin(); it != enemies->end() && i < maxTotalSize; it++, i++)
				{
					enemyAntsGroup->push_back(Ant(it->location.row, it->location.col));
					enemyAntsGroup->back().destination.row = it->location.row;
					enemyAntsGroup->back().destination.col = it->location.col;
				}
				break;
			}
			vector<pair<int, Ant>>::iterator nearestAnt;
			float shortestDist = 1000.0;
			for(auto it = enemies->begin(); it != enemies->end(); it++)
			{
				vector<pair<int, Ant>>::iterator nextMyAnt;								
				for(auto _ant = fightingAnts->begin(); _ant != fightingAnts->end(); _ant++)
				{
					if(_ant->first != -1 ||
						M::GetDirectDistance(it->location.row, it->location.col, _ant->second.location.row, 
						_ant->second.location.col) > FIND_NEAREST_ANT_COEFF * attackradius) 
					{
						continue;
					}
					float dist;
					if((dist = M::GetDirectDistance(_ant->second.location.row, _ant->second.location.col,
						myAntsGroup->back().location.row, myAntsGroup->back().location.col)) < shortestDist)
					{
						shortestDist = dist;
						nearestAnt = _ant;
					}
				}					
			}
			if(!nearestAnt._Ptr)
				break;
			myAntsGroup->push_back(nearestAnt->second);	
			myAntsGroup->back().destination.row = nearestAnt->second.location.row;
			myAntsGroup->back().destination.col = nearestAnt->second.location.col;
			nearestAnt->first = groupNumber;
			addedAnts++;
		}
		pairsGroup->push_back(make_pair(enemyAntsGroup, myAntsGroup));
		groupNumber++;
	}
	return pairsGroup;
}

vector<vector<Ant>*>* Bot::Attack(vector<pair<vector<Ant>*, vector<Ant>*>>* fightingAnts)
{	
	for_each(fightingAnts->begin(), fightingAnts->end(), [&](pair<vector<Ant>*, vector<Ant>*>& _pair){
		for_each(_pair.second->begin(), _pair.second->end(), [&](Ant& _ant){
			_ant.direction = 'F';
		});		
	});
	auto myAntsGroups = new vector<vector<Ant>*>();
	vector<Ant> *bestEmenyGroupConfig = NULL, *bestMyGroupConfig = NULL, *enemiesAntsGroup, *myAntsGroup;
	int myAntsGroupSize, enemyAntsGroupSize;
	for(auto _pair = fightingAnts->begin(); _pair != fightingAnts->end(); _pair++)
	{		
		if(_pair->first->size() == 0)
			continue;
		myAntsGroup = new vector<Ant>();
		*myAntsGroup = *(_pair->second);
		float totalBestMinimax = -10000.0;
		while(true)
		{
			if(myAntsGroup)
			{
				if(_pair->second->size() == 0)
					continue;
				bool myBestGroupIsInit = false;
				enemiesAntsGroup = new vector<Ant>();
				*enemiesAntsGroup = *(_pair->first);
				float bestMinimax = 10000.0;
				while(true)
				{
					if(enemiesAntsGroup)
					{
						float minimax = CalculateMinimax(enemiesAntsGroup, myAntsGroup);
						if(minimax <= totalBestMinimax)
						{
							bestMinimax = minimax;
							break;
						}					
						if(minimax < bestMinimax)
						{
							bestMinimax = minimax;
							if(bestEmenyGroupConfig)
								delete bestEmenyGroupConfig;	
							bestEmenyGroupConfig = enemiesAntsGroup;
						}
						else
						{
							delete enemiesAntsGroup;
						}
					}
					enemiesAntsGroup = GoNextGroupConfig(_pair->first, 0, 0);	
					if(!flag)
					{
						flag = true;
						break;
					}				
				}
				if (totalBestMinimax < bestMinimax)
				{
					totalBestMinimax = bestMinimax;
					bestMyGroupConfig = myAntsGroup;
				}
				else
					delete myAntsGroup;
			}
			myAntsGroup = GoNextGroupConfig(_pair->second, 0, 1);
			if(!flag)
			{
				flag = true;
				break;
			}			
		}
		myAntsGroups->push_back(bestMyGroupConfig);
	}
	return myAntsGroups;
}

vector<Ant>* Bot::GoNextGroupConfig(vector<Ant>* group, int mutableAnt, int groupType)
{	
	NextDirection((*group)[mutableAnt], group, groupType);	
	if((*group)[mutableAnt].direction == 'F')
	{
		if(mutableAnt == group->size() - 1)
		{
			flag = false;
			return false;
		}
		GoNextGroupConfig(group, mutableAnt + 1, groupType);		
	}
	if(mutableAnt == 0)
	{
		for(auto it = group->begin(); it != group->end(); it++)
		{
			if(count_if(group->begin(), group->end(), [&](Ant _ant){
				return it->destination.row == _ant.destination.row && it->destination.col == _ant.destination.col;
			}) == 2)
			{
				GoNextGroupConfig(group, mutableAnt + 1, groupType);
			}
		}
		auto nextGroup = new vector<Ant>();
		*nextGroup = *group;
		return nextGroup;
	}
	return NULL;
}

float Bot::CalculateMinimax(vector<Ant>* enemies, vector<Ant>* ants)
{
	auto myAntsSurrounding = new vector<int>();
	auto enemyAntsSurrounding = new vector<int>();
	float enemyDeadCount = 0, myDeadCount = 0;
	for(auto it = ants->begin(); it != ants->end(); it++)
	{
		int enemyCount = count_if(enemies->begin(), enemies->end(), [&](Ant ant){
			return M::GetDirectDistance(ant.destination.row, ant.destination.col, it->destination.row, it->destination.col) < attackradius;
		});
		it->distToTarget = enemyCount;
	}
	for(auto it = enemies->begin(); it != enemies->end(); it++)
	{
		int strongestAntSurrounding = 1000;
		int myAntsCount = count_if(ants->begin(), ants->end(), [&](Ant ant) -> bool{
			float dist = M::GetDirectDistance(ant.destination.row, ant.destination.col, it->destination.row, it->destination.col);
			if(ant.distToTarget < strongestAntSurrounding && dist < attackradius)
			{
				strongestAntSurrounding = ant.distToTarget;
			}
			return dist < attackradius;
		});
		if(strongestAntSurrounding <= myAntsCount)
		{
			enemyDeadCount++;
		}
		it->distToTarget = myAntsCount;
	}
	for(auto it = ants->begin(); it != ants->end(); it++)
	{
		if(find_if(enemies->begin(), enemies->end(), [&](Ant _ant) -> bool{
				float dist = M::GetDirectDistance(_ant.destination.row, _ant.destination.col, 
					it->destination.row, it->destination.col);
				return dist < attackradius && _ant.distToTarget <= it->distToTarget;
			}) != enemies->end())
		{
			myDeadCount++;
		}
	}
	float totalDistance = 0, sumDistance = 0, averageDistance = 0;
	/*int enemiesCount = enemies->size();
	for_each(ants->begin(), ants->end(), [&](Ant& m_ant){
		sumDistance = 0;
		for_each(enemies->begin(), enemies->end(), [&](Ant& e_ant){
			sumDistance += M::GetDirectDistance(e_ant.location.row, e_ant.location.col, 
												  m_ant.destination.row, m_ant.destination.col);
		});
		totalDistance += sumDistance / enemiesCount;
	});*/

	return 512 * enemyDeadCount - 512 * myDeadCount - totalDistance;
}

void Bot::NextDirection(Ant& ant, vector<Ant>* group, int antType)
{
	bool exit = false;
	while(!exit)
	{
		switch(ant.direction)
		{
		case 'F':
			ant.direction = 'N';
			break;
		case 'N':
			ant.direction = 'E';
			break;
		case 'E':
			ant.direction = 'W';
			break;
		case 'W':
			ant.direction = 'S';
			break;
		case 'S':
			ant.direction = 'F';
			break;
		}
		auto location = M::GetCoordOfDirection(ant.direction, ant.location.row, ant.location.col);
		ant.destination.row = location.row;
		ant.destination.col = location.col;
		exit =  			
			(find_if(water->begin(), water->end(), [&](Location tile){
				return location.row == tile.row && location.col == tile.col;
			}) == water->end());	
	}
	return;
}

vector<Ant>* Bot::GetEnemies(Ant& ant, vector<Ant>* enemiesGroup)
{
	auto enemies = new vector<Ant>();
	for(auto enemy = enemyAnts->begin(); enemy != enemyAnts->end(); enemy++)
	{
		float dist;
		if((dist = M::GetDirectDistance(ant.location.row, ant.location.col, 
			enemy->row, enemy->col)) < ATTACK_RADIUS_COEFF * attackradius &&
			find_if(enemiesGroup->begin(), enemiesGroup->end(), [&](Ant _enemy) -> bool {
				return enemy->row == _enemy.location.row && enemy->col == _enemy.location.col;
			}) == enemiesGroup->end())
		{
			enemies->push_back(Ant(enemy->row, enemy->col));
		}
	}	
	return enemies;
}

void Bot::UpdateAntsStateAccordingToAttackingGroup(vector<vector<Ant>*>* fightingGroups)
{
	int addedAnts = 0;
	for(auto _ant = myAnts->begin(); _ant != myAnts->end(); _ant++)
	{
		vector<Ant>::iterator found;
		for(auto group = fightingGroups->begin(); group != fightingGroups->end(); group++)
		{			
			if((found = find_if((*group)->begin(), (*group)->end(), [&](Ant fightingAnt){
				return 
					fightingAnt.location.row == _ant->location.row &&
					fightingAnt.location.col == _ant->location.col;
			})) != (*group)->end())		
			{
				_ant->direction = found->direction;
				_ant->target = ENEMY_ANTS;
				break;
			}
		}		
	}
}

vector<pair<int, Ant>>* Bot::FindFightingAnts()
{
	auto fightingAnts = new vector<pair<int, Ant>>();
	vector<Location>::iterator enemyAnt;
	float d;
	for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	{
		if((enemyAnt = find_if(enemyAnts->begin(), enemyAnts->end(), [&](Location ant) -> bool {
			d = M::GetDirectDistance(it->location.row, it->location.col, ant.row, ant.col);
			return d <= FIND_FIGHTING_ANTS_COEFF * attackradius; 
		})) != enemyAnts->end())
		{
			fightingAnts->push_back(make_pair(-1, *it));
		}
	}	
	return fightingAnts;
}

bool Bot::AllocTargetsForAnts(ofstream& out)
{
	out << "\n" << turn << endl;
	long timer;
	timer = clock();
	TargetIsReached();
	auto fightingAnts = FindFightingAnts();
	auto fightingGroups = DistributionForBattleGroups(fightingAnts);
	auto attackingGroups = Attack(fightingGroups);	

	UpdateAntsStateAccordingToAttackingGroup(attackingGroups);	
	out << (double)(clock() - timer)/CLOCKS_PER_SEC << endl;

	for(auto it = enemyHills->begin(); it != enemyHills->end(); it++)
	{
		for(auto ant = myAnts->begin(); ant != myAnts->end(); ant++)
		{			
			if((M::GetDirectDistance(it->row, it->col, ant->location.row, ant->location.col)) <= ENEMY_HILL_DISTANCE &&
				ant->target > ENEMY_ANTS)
			{	
				ant->target = ENEMY_HILL;
				ant->destination.row = it->row;
				ant->destination.col = it->col;
				ant->direction = M::SearchPathForAnt(ant._Ptr, this);
			}
		}
	}
	out << (double)(clock() - timer)/CLOCKS_PER_SEC << endl;
	for(auto it = food->begin(); it != food->end(); it++)
	{		
		M::SearchAntForTarget(it->row, it->col, this, FOOD);
	}
	out << (double)(clock() - timer)/CLOCKS_PER_SEC << endl;
	for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	{
		if(it->target == NOTHING)
		{
			it->target = EXPLORE;
			int rowMid = 0, colMid = 0;
			float dist;
			vector<SpaceForResearch>::iterator bestAltr;
			for(auto space = rschSpaces->begin(); space != rschSpaces->end(); space++)
			{
				dist = M::GetDirectDistance(space->row, space->col, it->location.row, it->location.col);
				if(dist < EXPLORE_RADIUS)
				{
					if(!bestAltr._Ptr || space->latestRsch > bestAltr->latestRsch || 
						(space->latestRsch == bestAltr->latestRsch && dist < bestAltr->dist))
					{
						if(count_if(myAnts->begin(), myAnts->end(), 
							[&](Ant &_ant){ return _ant.destination.row == space->row && _ant.destination.col == space->col; }) < SAME_TARGET_ANTS)
						{
							space->antsNumb++;
							bestAltr = space;
							bestAltr->dist = dist;
						}
					}
				}
			}
			if(bestAltr._Ptr)
			{
				it->destination.row = bestAltr->row;
				it->destination.col = bestAltr->col;
			}						
			M::SearchPathForAnt(it._Ptr, this);
		}
		else if(it->target == EXPLORE)
			M::SearchPathForAnt(it._Ptr, this);
	}
	out << (double)(clock() - timer)/CLOCKS_PER_SEC << endl;
	return true;
}

void Bot::TargetIsReached()
{
	int dist;
	for_each(myAnts->begin(), myAnts->end(), [&](Ant &ant){
		dist = M::GetDirectDistance(ant.location.row, ant.location.col, ant.destination.row, ant.destination.col);
		if(ant.target < EXPLORE)
		{
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.destination.row = -1;
			ant.destination.col = -1;
		}
		else if((ant.target == EXPLORE && dist <= TARGET_RADIUS) || ant.turn == TURNS_BEFORE_DISTRIB)
		{
			ant.turn = 0;
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.destination.row = -1;
			ant.destination.col = -1;
		}
	});
}

void Bot::MakeMoves(Target t, vector<Location>* nextTurnAnts, vector<Ant*> *immobilAnts)
{
	for(auto ant = myAnts->begin(); ant != myAnts->end(); ant++)
	{
		if(ant->target == t)
		{
			Location loc = M::GetCoordOfDirection(ant->direction, ant->location.row, ant->location.col);
			if(find_if(nextTurnAnts->begin(), nextTurnAnts->end(), [&](Location _loc)
				{ return _loc.row == loc.row && _loc.col == loc.col; }) == nextTurnAnts->end() &&
				find_if(myAnts->begin(), myAnts->end(), [&](Ant _ant)
				{ return _ant.location.row == loc.row && _ant.location.col == loc.col; }) == myAnts->end())
			{
				nextTurnAnts->push_back(loc);
				cout << "o " << ant->location.row << " " << ant->location.col << " " << ant->direction << endl;
				ant->ChangeLocation(ant->direction, rows, cols);
				ant->turn++;
			}
			else
			{
				nextTurnAnts->push_back(ant->location);
				immobilAnts->push_back(ant._Ptr);
			}
		}
	}
}

void Bot::TryMakeMoves(vector<Ant*> *immobilAnts)
{
	for(auto it = immobilAnts->begin(); it != immobilAnts->end(); it++)
	{
		if((*it)->target == ENEMY_ANTS)
			continue;
		int n = 0, rowTemp, colTemp, temp, dist;
		float rowMid = 0, colMid = 0;
		char dir[2];		
		for(auto _ant = immobilAnts->begin(); _ant != immobilAnts->end(); _ant++)
		{			
			if(dist = M::GetDirectDistance((**it).location.row, (**it).location.col, (**_ant).location.row, (**_ant).location.col) <= RADIUS)
			{
				temp = (**it).location.row - (**_ant).location.row;
				if(temp < (-1)*RADIUS)
					rowMid += (-1)*(temp + rows + 1);
				else if(temp >= (-1)*RADIUS && temp <= RADIUS)
					rowMid += (-1)*temp;				
				else
					rowMid += (-1)*(temp - rows - 1);

				temp = (**it).location.col - (**_ant).location.col;
				if(temp < (-1)*RADIUS)
					colMid += (-1)*(temp + cols + 1);
				else if(temp >= (-1)*RADIUS && temp <= RADIUS)
					colMid += (-1)*temp;				
				else
					colMid += (-1)*(temp - cols - 1);
				n++;
			}
		}
		rowMid /= (float)n;
		colMid /= (float)n;
		if(abs(rowMid) > abs(colMid))
		{
			if(rowMid > 0) dir[0] = 'N';
			else if(rowMid < 0) dir[0] = 'S';
			else dir[0] = 'R';
		}
		else
		{
			if(colMid > 0) dir[1] = 'W';
			else if(colMid < 0) dir[1] = 'E';
			else dir[1] = 'R';
		}
		for(int i = 0; i < 2; i++)
		{
			bool isW = true, isE = true, isN = true, isS = true;
			if(dir[i] != 'R')
			{
				Location loc = M::GetCoordOfDirection(dir[i], (**it).location.row, (**it).location.col);
				if(find_if(water->begin(), water->end(), [&](Location _loc){ return _loc.row == loc.row && _loc.col == loc.col; }) == water->end() &&
					find_if(myAnts->begin(), myAnts->end(), [&](Ant _ant){ return _ant.location.row == loc.row && _ant.location.col == loc.col; }) == myAnts->end())
				{
					cout << "o " << (**it).location.row << " " << (**it).location.col << " " << dir[i] << endl;
					(**it).ChangeLocation(dir[i], rows, cols);
					(**it).turn++;					
					break;
				}
			}			
		}		
	}
}

bool Bot::ParseInput(istream& in)
{
	vector<Ant> tempVector;
	enemyAnts->clear();	
	myHills->clear();
	food->clear();
	int row, col, player;
	string command, junk;
	while(1)
	{
		in >> command;
		if(command == "turn")
			in >> turn;			
		else if(command == "f")
		{
			in >> row >> col;
			food->push_back(Location(row, col));
		}
		else if(command == "w")
		{
			in >> row >> col;
			water->push_back(Location(row, col));
		}
		else if(command == "a")
		{
			in >> row >> col >> player;
			if(player == 0)
			{
				if(find_if(myAnts->begin(), myAnts->end(), [&](Ant ant){ 
					return (ant.location.row == row && ant.location.col == col); 
				}) == myAnts->end())
					myAnts->push_back(Ant(row, col));
				tempVector.push_back(Ant(row, col));
			}
			else
				enemyAnts->push_back(Location(row, col));
		}			
		else if(command == "h")
		{
			in >> row >> col >> player;
			if(player == 0)
				myHills->push_back(Location(row, col));
			else
			{
				if(find_if(enemyHills->begin(), enemyHills->end(), [&](Location hill){
					return (hill.row == row && hill.col == col);
				}) == enemyHills->end())
				{
					enemyHills->push_back(Location(row, col));
				}
			}
		}
		else if(command == "go")
		{
			myAnts->erase(remove_if(myAnts->begin(), myAnts->end(), [&](Ant ant){
				return (find_if(tempVector.begin(), tempVector.end(), [&](Ant tant){
					return ant.location.row == tant.location.row && ant.location.col == tant.location.col;
				}) == tempVector.end());
			}), myAnts->end());
			return true;
		}		
		else if(command == "end")
		{
			return false;
		}
		else
			getline(in, junk);
	}
	return true;
}

void Bot::ParseInitialInput(istream& in)
{
	string command;
	while(1)
	{
		in >> command;
		if(command == "turn")
			in >> turn;
		else if(command == "ready")
			break;
		else if(command[0] == 'l')
			in >> loadtime;
		else if(command[0] == 't')
			in >> turntime;
		else if(command[0] == 'r')
			in >> rows;
		else if(command[0] == 'c')
			in >> cols;
		else if(command[0] == 'v')
			in >> viewradius;
		else if(command[0] == 'a')
			in >> attackradius;
		else if(command[0] == 's')
			in >> randomSeed;
	}
	attackradius = sqrt(attackradius) + 0.001;
	viewradius = sqrt(viewradius) + 0.001;
}