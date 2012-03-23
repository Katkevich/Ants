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
	//ParseInitialInput();
	//cout << "go" << endl;
	//int i = 0, j = 2, r = 1, c = 999999999;
	int row, col, player;
	//Parse();
	ifstream in("input.txt");
	//in.open("input.txt");
	ParseInitialInput(cin);
	//Movement::init(rows, cols);
	Movement::maxRows = rows;
	Movement::maxCols = cols;	

	for(int i = RES_SQUARE; i < rows; i += 2 * RES_SQUARE)
		for(int j = RES_SQUARE; j < cols; j += 2 * RES_SQUARE)
			rschSpaces->push_back(SpaceForResearch(i, j, 1));

	cout << "go" << endl;
	//fstream str;
	//str.open("output.txt", ios::out);
	fstream stream;
	stream.open("output.txt", ios::out|ios::trunc);	
	do
	{	
		long c;
		c = clock();
		
		exit = ParseInput(cin);

		for_each(rschSpaces->begin(), rschSpaces->end(), [&](SpaceForResearch &space){
			if(find_if(myAnts->begin(), myAnts->end(), [&](Ant _ant){ 
				return Movement::GetDirectDistance(space.row, space.col, _ant.location.row, _ant.location.col) < viewradius;
			}) != myAnts->end())
				space.latestRsch = 0;
		});

		AllocTargetsForAnts();
		MakeMoves();

		for_each(rschSpaces->begin(), rschSpaces->end(), [](SpaceForResearch &space){
			space.latestRsch++;
		});

		/*for_each(myAnts->begin(), myAnts->end(), [](Ant &ant){
			ant.distToTarget = 40000;
			ant.target = EXPLORE;
		});*/
		c = clock() - c;
		cout << turn << " " << (double)c/CLOCKS_PER_SEC << endl;
	//	char next = Movement::SearchPathForAnt(/*rows, cols, */&((*myAnts)[0]), this);

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
	in.close();
	stream.close();
}

void Bot::AllocTargetsForAnts()
{	
	TargetIsReached();
	for(auto it = food->begin(); it != food->end(); it++)
	{
		Movement::SearchAntForTarget(it->row, it->col, this, FOOD);
	}
	for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	{
		/*if(it->target == EXPLORE && 
			Movement::GetDirectDistance(it->location.row, it->location.col, it->destination.row, it->destination.col) <= TARGET_RADIUS)
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
				dist = Movement::GetDirectDistance(space.row, space.col, it->location.row, it->location.col);
				if(dist < EXPLORE_RADIUS)
				{

				}
			});*/
			vector<SpaceForResearch>::iterator bestAltr;
			for(auto space = rschSpaces->begin(); space != rschSpaces->end(); space++)
			{
				dist = Movement::GetDirectDistance(space->row, space->col, it->location.row, it->location.col);
				if(dist < EXPLORE_RADIUS)
				{
					if(!bestAltr._Ptr || space->latestRsch > bestAltr->latestRsch || 
						(space->latestRsch == bestAltr->latestRsch && dist < bestAltr->dist))
					{
						bestAltr = space;
						bestAltr->dist = dist;
						//continue;
					}
				}
			}
			it->destination.row = bestAltr->row;
			it->destination.col = bestAltr->col;
			//for(Ant &ant : myAnts) 
			/*for_each(myAnts->begin(), myAnts->end(), [&rowMid, &colMid](Ant &_ant)
			{ 
				rowMid += _ant.location.row; 
				colMid += _ant.location.col; 
			});
			rowMid /= myAnts->size();
			colMid /= myAnts->size();
			int tempX, tempY;
			float d = Movement::GetDirectDistance(rowMid, colMid, it->location.row, it->location.col);
			tempX = (DIST_TO_TARGET / d)*(it->location.row - rowMid) + it->location.row;
			tempY = (DIST_TO_TARGET / d)*(it->location.col - colMid) + it->location.col;
			if(tempX < 0) tempX += rows;
			else if(tempX >= rows) tempX -= rows;
			if(tempY < 0) tempY += cols;
			else if(tempY >= cols) tempY -= cols;
			it->destination.row = tempX;
			it->destination.col = tempY;*/
			int u;
			/*{
				colMid += ant.location.col;
				rowMid += ant.location.row;
			}*/
			//!!!!!!!			
			srand(time(0));
	/*		int temp = rand() % (rschSpaces->size() - 1);
			SpaceForResearch* bestAlt = &((*rschSpaces)[temp]);
			int minDist = Movement::GetDirectDistance((*rschSpaces)[temp].row, (*rschSpaces)[temp].col, it->location.row, it->location.col);
			for_each(rschSpaces->begin(), rschSpaces->end(), [&](SpaceForResearch &space){
				if(space.latestRsch + space.antsNumb < bestAlt->latestRsch + bestAlt->antsNumb || 
					(space.latestRsch + space.antsNumb == bestAlt->latestRsch + bestAlt->antsNumb &&
					(temp = Movement::GetDirectDistance(space.row, space.col, it->location.row, it->location.col)) < minDist))
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
		//	it->destination.row = 37;
		//	it->destination.col = 50;
			Movement::SearchPathForAnt(it._Ptr, this);
	//		}
			
		}
		else if(it->target == EXPLORE)
			Movement::SearchPathForAnt(it._Ptr, this);
	}
}

void Bot::TargetIsReached()
{
	int dist;
	for_each(myAnts->begin(), myAnts->end(), [&](Ant &ant){
		dist = Movement::GetDirectDistance(ant.location.row, ant.location.col, ant.destination.row, ant.destination.col);
		if(ant.target == FOOD/* && dist <= 1*/)
		{
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.onTheWay = false;
		}
		else if(ant.target == EXPLORE && dist <= TARGET_RADIUS)
		{
			ant.target = NOTHING;
			ant.distToTarget = 40000;
			ant.onTheWay = false;
		}
	});
}

void Bot::MakeMoves()
{
	for(auto it = myAnts->begin(); it != myAnts->end(); it++)
	{
		//if(it->target == FOOD)
		cout << "o " << it->location.row << " " << it->location.col << " " << it->direction << endl;
		//else
		//	cout << "o " << it->location.row << " " << it->location.col << " " << it->direction << "++++++++++\n";
		it->ChangeLocation(it->direction, rows, cols);
	}
	cout << "go\n";
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
	enemyHills->clear();
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
				enemyHills->push_back(Location(row, col));
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