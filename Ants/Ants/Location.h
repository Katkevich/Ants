#pragma once

struct Location
{
public:
	int row;
	int col;

	Location()
	{
		row = col = 0;
	}

	Location(int row, int col)
	{
		this->row = row;
		this->col = col;
	}
};
