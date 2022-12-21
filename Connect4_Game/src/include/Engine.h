#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Configuration.h"

#define ROWS config.height
#define COLS config.width
#define SCREEN_WIDTH (COLS * 80)
#define SCREEN_HEIGHT (ROWS * 80 + 100)
#define CIRCLE_SIZE 70

typedef struct
{
	char** board;
	bool p1First;
	
	int* log;
	int currMove, totalMoves;
} Game;

typedef struct
{
	char color;
	int score;
} Player;

Player p1, p2;

Game createNewGame();

void initializeBoard(char** board);
void makeMove(Game* game, int col);
void undoMove(Game* game);
void redoMove(Game* game);
void updateScore(Game* game, Player* player, int col, int increment);
int calcVertically(Game* game, int c);
int calcHorizontally(Game* game, int c);
int calcDiagonally(Game* game, int c);
void endGame(Game* game);
