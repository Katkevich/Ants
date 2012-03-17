#include "StdAfx.h"
#include "Bot.h"

Bot::Bot()
{
	enemyHills = new vector<Location>();
	enemyAnts = new vector<Location>();
	myAnts = new vector<Ant>();
	myHills = new vector<Location>();
	water = new vector<Location>();
	food = new vector<Location>();
}

Bot::~Bot()
{
	delete enemyAnts; enemyAnts = NULL;
	delete enemyHills; enemyHills = NULL;
	delete myHills; myHills = NULL;
	delete myAnts; myAnts = NULL;
	delete water; water = NULL;
	delete food; food = NULL;
}
//
//bool Bot::Parse()
//{
//	int row, col, player;
//	string command, junk;	
//	while(cin >> command)
//	{
//		if(command == "end")
//		{
//			gameover = 1;
//			break;
//		}
//		else if(command == "turn")
//		{
//			cin >> turn;
//			break;
//		}
//		else
//			getline(cin, junk);
//	}
//	if (turn == 0)
//	{
//		while(cin >> command)
//		{		
//			if(command == "loadtime")
//				cin >> loadtime;
//			else if(command == "turntime")
//				cin >> turntime;
//			else if(command == "rows")
//				cin >> rows;
//			else if(command == "cols")
//				cin >> cols;
//			else if(command == "turns")
//				cin >> turns;
//			else if(command == "player_seed")
//				cin >> seed;
//			else if(command == "viewradius2")
//			{
//				cin >> viewradius;
//				viewradius = sqrt(viewradius);
//			}
//			else if(command == "attackradius2")
//			{
//				cin >> attackradius;
//				attackradius = sqrt(attackradius);
//			}
//			else if(command == "spawnradius2")
//			{
//				cin >> spawnradius;
//				spawnradius = sqrt(spawnradius);
//			}
//			else if(command == "ready") //end of parameter input
//			{
//				break;
//			}
//			else    //unknown line
//				getline(cin, junk);			
//		}
//	}
//	else
//	{
//		while(cin >> command)
//		{			
//			if(command[0] == 'f')
//			{
//				cin >> row >> col;
//				food->push_back(Location(row, col));
//			}
//			else if(command[0] == 'w')
//			{
//				cin >> row >> col;
//				water->push_back(Location(row, col));
//			}
//			else if(command[0] == 'a')
//			{
//				cin >> row >> col >> player;
//				if(player == 0)
//					myAnts->push_back(Ant(row, col));
//				else
//					enemyAnts->push_back(Location(row, col));
//			}			
//			else if(command[0] == 'h')
//			{
//				cin >> row >> col >> player;
//				if(player == 0)
//					myHills->push_back(Location(row, col));
//				else
//					enemyHills->push_back(Location(row, col));
//			}
//			else if(command == "go")
//			{
//				if(gameover)
//					cin.setstate(ios::failbit);
//				break;		
//			}
//			else if(command == "players")
//				cin >> players;
//			/*else if(command == "scores")
//			{
//				scores = vector<double>(players, 0.0);
//				for(int i = 0; i < players; i++)
//					cin >> scores[i];
//			}*/
//			else
//				getline(cin, junk);
//		}
//	}
//	return true;
//}
//

void Bot::Run()
{
	bool exit;	
	//ParseInitialInput();
	//cout << "go" << endl;
	int i = 0, j = 2, r = 1, c = 999999999;
	int row, col, player;
	//Parse();
	ifstream in("input.txt");
	//in.open("input.txt");
	ParseInitialInput(cin);		
	cout << "go" << endl;
	//fstream str;
	//str.open("output.txt", ios::out);
	fstream stream;
	stream.open("output.txt", ios::out|ios::trunc);	
	do
	{		
		/*string command, junk;
		cin >> command;
		if(command == "turn")
			cin >> turn;
		else if (command == "go")
			cout << "go\n";			
		else if(command == "f")
		{
			cin >> row >> col;
			food->push_back(Location(row, col));
		}
		else if(command == "w")
		{
			cin >> row >> col;
			water->push_back(Location(row, col));
		}
		else if(command == "a")
		{
			cin >> row >> col >> player;
			if(player == 0)
				myAnts->push_back(Ant(row, col));
			else
				enemyAnts->push_back(Location(row, col));
		}			
		else if(command == "h")
		{
			cin >> row >> col >> player;
			if(player == 0)
				myHills->push_back(Location(row, col));
			else
				enemyHills->push_back(Location(row, col));
		}
		else if(command == "end")
			break;
		else if(command == "players")
			cin >> players;
		else if(command == "scores")
		{
			scores = vector<double>(players, 0.0);
			for(int i = 0; i < players; i++)
				cin >> scores[i];
		}
		else
			getline(cin, junk);*/
		exit = ParseInput(cin);		
		//cout << "go\no " << myAnts[0].location.col << " " << myAnts[0].location.row << " go\n";
		//i = myAnts.size();
		//r = myAnts[i].location.row;
		//c = myAnts[i].location.col;
		//str << myAnts[i].location.row << " " << myAnts[i].location.col << endl;
		(*myAnts)[0].destination.row = 58;
		(*myAnts)[0].destination.col = 49;
		(*myAnts)[0].target = EXPLORE;
		/*(*myAnts)[0].destination.row = 1;
		(*myAnts)[0].destination.col = 1;
		(*myAnts)[0].target = FOOD;
*/
		//(*myAnts)[i].MakeMove(water, 6, 8);		
		char next = (*myAnts)[0].MakeMove(rows, cols, water);		
		cout << "o " << (*myAnts)[0].location.row << " " << (*myAnts)[0].location.col << " " << next <<"\ngo\n";	
		stream << cout << "o " << (*myAnts)[0].location.row << " " << (*myAnts)[0].location.col << " " << next <<"\ngo\n";
		(*myAnts)[0].ChangeLocation(next);
		//if (myAnts[0].location.col == myAnts[myAnts.size() - 1].location.col)
			//cout << "go\no " << "32" << " " << myAnts[0].location.col << " W\ngo\n";	
		//(*myAnts)[0].location.col--;
		//cout << "go" << endl;
		//cout << "o 32 " << i << " W\ngo\n";
		i++;
	} while(exit);
	in.close();
	stream.close();
	//str.close();
	//cout << "go" << endl;
		//exit = ParseInput();
		
		//Parse();		
//		cout << "go" << endl;
//	//	if (i > 0)
			//cout << "go\n" << myAnts[0].location.row << " " << myAnts[0].location.col << " W\ngo";		
////		else
//			//cout << myAnts[0].location.row << " " << myAnts[0].location.row << " S\ngo";		
//		cout << "go" << endl;
//	} while(cin.failbit == 0);
}

void Bot::SetTargets()
{
	for(auto it = food->begin(); it != food->end(); it++)
	{
		SearchAnt((*it));
	}
}

void Bot::SearchAnt(Location target)
{

}

void Bot::EndMove()
{

}

bool Bot::ParseInput(istream& in)
{
	//fstream stream;
	//stream.open("output.txt", ios::out);
	enemyAnts->clear();	
	myHills->clear();
	enemyHills->clear();
	int row, col, player;
	string command, junk;
	//fstream in;
	//in.open("input.txt", ios::in);
	while(1)
	{
		//cin >> command;
		in >> command;
		if(command == "turn")
			in >> turn;			
		//cin >> turn;			
		else if(command == "f")
		{
			//cin >> row >> col;
			in >> row >> col;
			food->push_back(Location(row, col));
		}
		else if(command == "w")
		{
			//cin >> row >> col;
			in >> row >> col;
			water->push_back(Location(row, col));
		}
		else if(command == "a")
		{
			//cin >> row >> col >> player;
			in >> row >> col >> player;
			if(player == 0)
			{
				myAnts->push_back(Ant(row, col));
				//stream << row << " " << col << endl;
			}
			else
				enemyAnts->push_back(Location(row, col));
		}			
		else if(command == "h")
		{
			//cin >> row >> col >> player;
			in >> row >> col >> player;
			if(player == 0)
				myHills->push_back(Location(row, col));
			else
				enemyHills->push_back(Location(row, col));
		}
		else if(command == "go")
		{
			//stream.close();
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
			//stream.close();
			return false;
		}
		else
			getline(in, junk);
	}
	//stream.close();
	//in.close();
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