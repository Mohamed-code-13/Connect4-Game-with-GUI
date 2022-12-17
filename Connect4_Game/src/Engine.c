#include "include/Engine.h"

Player p1 = { 'r', 0 }, p2 = { 'g', 0 };

Game createNewGame()
{
	Game game;
	game.p1First = true;
	
	initializeBoard(game.board);

	return game;
}

void initializeBoard(char board[ROWS][COLS])
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
			board[i][j] = '-';
}

void makeMove(Game* game, int col)
{
	for (int i = ROWS - 1; i >= 0; i--)
	{
		if (game->board[i][col] == '-')
		{
			if (game->p1First)
				game->board[i][col] = p1.color;
			else
				game->board[i][col] = p2.color;

			game->p1First = !(game->p1First);
			return;
		}
	}
}