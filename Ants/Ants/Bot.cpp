#include "StdAfx.h"
#include "Bot.h"
#include "Movement.h"

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

		if(turn == 44)
		{
			int uy = 0;
			uy++;
		}
		AllocTargetsForAnts();		
		
		MakeMoves(ENEMY_HILL, nextTurnAnts, immobilAnts);
		MakeMoves(FOOD, nextTurnAnts, immobilAnts);
		MakeMoves(EXPLORE, nextTurnAnts, immobilAnts);
		TryMakeMoves(immobilAnts);
		
		enemyHills->erase(remove_if(enemyHills->begin(), enemyHills->end(), [&](Location hill){
			return(find_if(myAnts->begin(), myAnts->end(), [&](Ant ant){
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
	//	char next = M::SearchPathForAnt(/*rows, cols, */&((*myAnts)[0]), this);

		//char next = (*myAnts)[0].MakeMove(rows, cols, water);		

	//	cout << "o " << (*myAnts)[0].location.row << " " << (*myAnts)[0].location.col << " " << next <<"\ngo\n";	

//		stream << cout << "o " << (*myAnts)[0].location.row << " " << (*myAnts)[0].location.col << " " << next <<"\ngo\n";
	//	(*myAnts)[0].ChangeLocation(next, rows, cols);
		//if (myAnts[0].location.col == myAnts[myAnts.size() - 1].location.col)
			//cout << "go\no " << "32" << " " << myAnts[0].location.col << " W\ngo\n";	
		//(*myAnts)[0].location.col--;
		//cout << "go" << endl;
		//cout << "o 32 " << i << " W\ngo\n";
//		/i++;
	} while(exit);
	delete nextTurnAnts;
	delete immobilAnts;
	in.close();
}

void Bot::AllocTargetsForAnts()
{	
	TargetIsReached();
	for(auto it = enemyHills->begin(); it != enemyHills->end(); it++)
	{
		for(auto ant = myAnts->begin(); ant != myAnts->end(); ant++)
		{
			if(M::GetDistance(it->row, it->col, ant->location.row, ant->location.col) <= ENEMY_HILL_DISTANCE)
			{				
				ant->target = ENEMY_HILL;
				ant->destination.row = it->row;
				ant->destination.col = it->col;
				ant->direction = M::SearchPathForAnt(ant._Ptr, this);				
			}
		}
	}
	for(auto it = food->begin(); it != food->end(); it++)
	{		
		M::SearchAntForTarget(it->row, it->col, this, FOOD);
	}
	for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	{
		/*if(it->target == EXPLORE && 
			M::GetDirectDistance(it->location.row, it->location.col, it->destination.row, it->destination.col) <= TARGET_RADIUS)
			it->onTheWay = false;*/
		//if(it->target == EXPLORE)
		if(it->target == NOTHING)
		{
			it->target = EXPLORE;
	//		if(it->onTheWay == false)
	//		{
			//int rowMid = accumulate(myAnts->begin(), myAnts->end(), 0, [](vector<Ant>::iterator a1, vector<Ant>::iterator a2){ return a1->location.row + a2->Location.row; });			
			int rowMid = 0, colMid = 0;
			float dist;
			/*for_each(rschSpaces->begin(), rschSpaces->end(), [&](SpaceForResearch &space){
				dist = M::GetDirectDistance(space.row, space.col, it->location.row, it->location.col);
				if(dist < EXPLORE_RADIUS)
				{

				}
			});*/
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
						//continue;
					}
				}
			}
			if(bestAltr._Ptr)
			{
				it->destination.row = bestAltr->row;
				it->destination.col = bestAltr->col;
			}
			//for(Ant &ant : myAnts) 
			/*for_each(myAnts->begin(), myAnts->end(), [&rowMid, &colMid](Ant &_ant)
			{ 
				rowMid += _ant.location.row; 
				colMid += _ant.location.col; 
			});
			rowMid /= myAnts->size();
			colMid /= myAnts->size();
			int tempX, tempY;
			float d = M::GetDirectDistance(rowMid, colMid, it->location.row, it->location.col);
			tempX = (DIST_TO_TARGET / d)*(it->location.row - rowMid) + it->location.row;
			tempY = (DIST_TO_TARGET / d)*(it->location.col - colMid) + it->location.col;
			if(tempX < 0) tempX += rows;
			else if(tempX >= rows) tempX -= rows;
			if(tempY < 0) tempY += cols;
			else if(tempY >= cols) tempY -= cols;
			it->destination.row = tempX;
			it->destination.col = tempY;*/
			int u;
			//!!!!!!!			
			srand(time(0));
	/*		int temp = rand() % (rschSpaces->size() - 1);
			SpaceForResearch* bestAlt = &((*rschSpaces)[temp]);
			int minDist = M::GetDirectDistance((*rschSpaces)[temp].row, (*rschSpaces)[temp].col, it->location.row, it->location.col);
			for_each(rschSpaces->begin(), rschSpaces->end(), [&](SpaceForResearch &space){
				if(space.latestRsch + space.antsNumb < bestAlt->latestRsch + bestAlt->antsNumb || 
					(space.latestRsch + space.antsNumb == bestAlt->latestRsch + bestAlt->antsNumb &&
					(temp = M::GetDirectDistance(space.row, space.col, it->location.row, it->location.col)) < minDist))
				{
					bestAlt = &space;
					minDist = temp;					
				}
			});
			bestAlt->antsNumb++;
			bestAlt->latestRsch = 0;*/
			//!!!!!!!
			it->onTheWay == true;
			//int rowMid = accumulate(myAnts->begin(), myAnts->end(), 0, function(Ant a1, Ant a2){});
			M::SearchPathForAnt(it._Ptr, this);
		}
		else if(it->target == EXPLORE)
			M::SearchPathForAnt(it._Ptr, this);
	}
}

void Bot::TargetIsReached()
{
	int dist;
	for_each(myAnts->begin(), myAnts->end(), [&](Ant &ant){
		dist = M::GetDirectDistance(ant.location.row, ant.location.col, ant.destination.row, ant.destination.col);
		if(ant.target == FOOD || ant.target == ENEMY_HILL)
		{
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.onTheWay = false;
			ant.destination.row = -1;
			ant.destination.col = -1;
		}
		else if((ant.target == EXPLORE && dist <= TARGET_RADIUS) || ant.turn == TURNS_BEFORE_DISTRIB)
		{
			ant.turn = 0;
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.onTheWay = false;
			ant.destination.row = -1;
			ant.destination.col = -1;
		}
	});
}

void Bot::MakeMoves(Target t, vector<Location>* nextTurnAnts, vector<Ant*> *immobilAnts)
{
	//for_each(myAnts->begin(), myAnts->end(), [&](Ant &_ant)
	//{
	//	if(_ant.target == t)
	//	{
	//		Location loc = M::GetCoordOfDirection(_ant.direction, _ant.location.row, _ant.location.col);
	//		if(find_if(nextTurnAnts->begin(), nextTurnAnts->end(), [&](Location _loc){ return _loc.row == loc.row && _loc.col == loc.col; }) == nextTurnAnts->end() &&
	//			find_if(myAnts->begin(), myAnts->end(), [&](Ant _ant){ return _ant.location.row == loc.row && _ant.location.col == loc.col; }) == myAnts->end())
	//		{
	//			nextTurnAnts->push_back(loc);
	//			cout << "o " << _ant.location.row << " " << _ant.location.col << " " << _ant.direction << endl;
	//			_ant.ChangeLocation(_ant.direction, rows, cols);
	//			_ant.turn++;
	//		}
	//		else
	//		{
	//			nextTurnAnts->push_back(_ant.location);
	//			immobilAnts->push_back(&_ant);
	//		}
	//	}
	//});

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

	//for_each(myAnts->begin(), myAnts->end(), [&](Ant &_ant)
	//{
	//	if(_ant.target == EXPLORE)
	//	{
	//		Location loc = M::GetCoordOfDirection(_ant.direction, _ant.location.row, _ant.location.col);
	//		if(find_if(turnsVector.begin(), turnsVector.end(), [&](Location _loc){ return  _loc.row == loc.row && _loc.col == loc.col; }) == turnsVector.end())
	//		{
	//			turnsVector.push_back(loc);
	//			cout << "o " << _ant.location.row << " " << _ant.location.col << " " << _ant.direction << endl;
	//			_ant.ChangeLocation(_ant.direction, rows, cols);
	//			_ant.turn++;
	//		}
	//	}
	//});

	//for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	//{
	//	//if(it->target == FOOD)
	//	cout << "o " << it->location.row << " " << it->location.col << " " << it->direction << endl;
	//	//else
	//	//	cout << "o " << it->location.row << " " << it->location.col << " " << it->direction << "++++++++++\n";
	//	it->ChangeLocation(it->direction, rows, cols);
	//	it->turn++;
	//}
//	cout << "go\n";
}

void Bot::TryMakeMoves(vector<Ant*> *immobilAnts)
{
	for(auto it = immobilAnts->begin(); it != immobilAnts->end(); it++)
	{
		int n = 0, rowTemp, colTemp, temp, dist;
		float rowMid = 0, colMid = 0;
		char dir[2];
		/*for_each(immobilAnts->begin(), immobilAnts->end(), [&](Ant *_ant)
		{			
			if(dist = M::GetDirectDistance((**it).location.row, (**it).location.col, _ant->location.row, _ant->location.col) <= RADIUS)
			{
				temp = (**it).location.row - _ant->location.row;
				if(temp < (-1)*RADIUS)
					rowMid += (-1)*(temp + rows + 1);
				else if(temp >= (-1)*RADIUS && temp <= RADIUS)
					rowMid += (-1)*temp;				
				else
					rowMid += (-1)*(temp - rows - 1);

				temp = (**it).location.col - _ant->location.col;
				if(temp < (-1)*RADIUS)
					colMid += (-1)*(temp + cols + 1);
				else if(temp >= (-1)*RADIUS && temp <= RADIUS)
					colMid += (-1)*temp;				
				else
					colMid += (-1)*(temp - cols - 1);
				n++;
			}
		});*/
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
					/*(**it).target = NOTHING;
					(**it).distToTarget = 40000;
					(**it).onTheWay = false;
					(**it).destination.row = -1;
					(**it).destination.col = -1;*/
					break;
				}
			}
			//else if(i == 0)
			//{
			//	while(isN || isE || isW || isS)
			//	{
			//		srand(time(0));
			//		int id = rand() % 4;
			//		switch(id)
			//		{
			//		case 0:
			//			isN = false;
			//			break;
			//		case 1:
			//			isE = false;
			//			break;
			//		case 2:
			//			isW = false;
			//			break;
			//		case 3:
			//			isS = false;
			//			break;
			//		}
			//		if(id == 0 && isN && M::CanMove(DIRECTION[id], (**it).location.row, (**it).location.col, this))
			//		{
			//			cout << "o " << (**it).location.row << " " << (**it).location.col << " " << 'N' << endl;
			//			(**it).ChangeLocation(dir[i], rows, cols);
			//			(**it).turn++;
			//			//goto exit;
			//		}
			//	}
			//}
		}
		//exit:;
	}
}

void Bot::EndMove()
{

}

bool Bot::ParseInput(istream& in)
{
	vector<Ant> tempVector;
	enemyAnts->clear();	
//	myAnts->clear();
	myHills->clear();
//	enemyHills->clear();
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
		/*else if(command == "players")
			cin >> players;*/
		/*else if(command == "scores")
		{
			scores = vector<double>(players, 0.0);
			for(int i = 0; i < players; i++)
				cin >> scores[i];
		}*/
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
			in >> seed;
	}
	attackradius = sqrt(attackradius);
	viewradius = sqrt(viewradius);
}