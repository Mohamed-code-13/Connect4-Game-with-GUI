#pragma once
#include <stdbool.h>

#define ROWS 7
#define COLS 9
#define SCREEN_WIDTH (COLS * 100)
#define SCREEN_HEIGHT (ROWS * 100)
#define CIRCLE_SIZE 80

typedef struct
{
	char board[ROWS][COLS];
	bool p1First;
} Game;

typedef struct
{
	char color;
	int score;
} Player;

Player p1, p2;

Game createNewGame();

void initializeBoard(char board[ROWS][COLS]);
void makeMove(Game* game, int col);