// Ants.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bot.h"


int _tmain(int argc, _TCHAR* argv[])
{
	cout.sync_with_stdio(0);
	Bot *bot = new Bot();
	bot->Run();
	delete bot;
	return 0;
}

