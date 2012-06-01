#pragma once

/**
 *	Responsible for the location of the cell.
 */
struct Location
{
public:
	/**
	 *	The row number.
	 */
	int row;

	/**
	 *	The column number.
	 */
	int col;

	/**
	 *	Constructor.
	 */
	Location()
	{
		row = col = 0;
	}

	/**
	 *	Constructor.
	 *  @param row Initial row number.
	 *  @param col Initial column number.
	 */
	Location(int row, int col)
	{
		this->row = row;
		this->col = col;
	}
};
