#include "include/computer.h"

/*
* The computer Algorithm:
*   1 - Try to play above the previous play.
*   2 - If that can't happen. Try play at the right of that play.
*   3 - If that can't happen. Try play at the left of that play.
*/

int getColLeft(char** board, int rows, int c)
{
	if (c == -1)
		return -1;

	for (int i = 0; i < rows; ++i)
	{
		if (board[i][c] == '-')
			return c;
	}

	return getColLeft(board, rows, c - 1);
}

int getColRight(char** board, int rows, int cols, int c)
{
	if (c == cols)
		return -1;

	for (int i = 0; i < rows; ++i)
	{
		if (board[i][c] == '-')
			return c;
	}

	return getColRight(board, rows, cols, c + 1);
}

int getCol(char** board, int rows, int cols, int c)
{
	int l = getColLeft(board, rows, c);
	int r = getColRight(board, rows, cols, c);

	return (l != -1) ? l : r;

}