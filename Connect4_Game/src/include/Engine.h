#pragma once
#include <stdio.h>
#include <stdbool.h>

#define ROWS 7
#define COLS 9
#define SCREEN_WIDTH (COLS * 80)
#define SCREEN_HEIGHT (ROWS * 80 + 100)
#define CIRCLE_SIZE 70

typedef struct
{
	char board[ROWS][COLS];
	bool p1First;

	int log[ROWS * COLS];
	int currMove, totalMoves;
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
void undoMove(Game* game);
void redoMove(Game* game);
void updateScore(Game* game, Player* player, int col, int increment);
int calcVertically(Game* game, int c);
int calcHorizontally(Game* game, int c);
int calcDiagonally(Game* game, int c);
void endGame(Game* game);
