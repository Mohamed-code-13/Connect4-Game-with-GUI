#pragma once

#define ROWS 6
#define COLS 7

typedef struct
{
	int mRow;
	int mCol;
} Move;

typedef struct
{
	char board[ROWS][COLS];
	// Move log[10];
	int logIx;
} Game;

Game createNewGame();

void cleanBoard(Game* g);