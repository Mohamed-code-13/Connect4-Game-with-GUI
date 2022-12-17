#include "include/Engine.h"

Game createNewGame()
{
	Game game;
	game.logIx = 0;

	cleanBoard(game.board);

	return game;
}

void cleanBoard(char board[ROWS][COLS])
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
			board[i][j] = '-';
}