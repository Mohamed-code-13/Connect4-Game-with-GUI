#include "include/Engine.h"

Player p1 = { 'r', 0 }, p2 = { 'g', 0 };

Game createNewGame()
{
	Game game;
	game.p1First = true;
	game.currMove = 0;
	game.totalMoves = 0;
	
	initializeBoard(game.board);

	return game;
}

void initializeBoard(char board[ROWS][COLS])
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
			board[i][j] = '-';
}

void flipPlayer(Game* game)
{
	game->p1First = !(game->p1First);
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

			flipPlayer(game);
			
			if (game->totalMoves < game->currMove || game->log[game->currMove] != col)
				game->totalMoves = game->currMove + 1;
			game->log[game->currMove++] = col;

			return;
		}
	}
}

void undoMove(Game* game)
{
	if (game->currMove == 0)
		return;

	int c = game->log[--(game->currMove)];
	for (int i = 0; i < ROWS; ++i)
	{
		if (game->board[i][c] != '-')
		{
			game->board[i][c] = '-';
			break;
		}
	}
	flipPlayer(game);
}

void redoMove(Game* game)
{
	if (game->totalMoves == 0 || game->totalMoves == game->currMove)
		return;

	int c = game->log[game->currMove];
	
	makeMove(game, c);
}