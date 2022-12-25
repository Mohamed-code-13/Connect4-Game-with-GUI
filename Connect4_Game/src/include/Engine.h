#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Configuration.h"
#include "high_score.h"
#include "ai.h"

#define ROWS config.height
#define COLS config.width
#define SCORES config.highScore
#define CIRCLE_SIZE 70
#define SCREEN_WIDTH (COLS * (CIRCLE_SIZE + 10))
#define SCREEN_HEIGHT (ROWS * (CIRCLE_SIZE + 10) + 100)

typedef struct
{
	char** board;
	bool p1First;
	bool ai;

	int* log;
	int currMove, totalMoves;

	bool gameEnded;
} Game;

typedef struct
{
	char color;
	int score;
	char name[20];
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
int endGame(Game* game);
