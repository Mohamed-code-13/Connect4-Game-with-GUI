#include "include/Engine.h"

Player p1 = { 'r', 0 }, p2 = { 'g', 0 };

Game createNewGame()
{
	Game game;

	game.p1First = true;
	game.ai = false;
	game.currMove = 0;
	game.totalMoves = 0;

	game.board = (char**)malloc(ROWS * sizeof(char*));
	game.log = (int*)malloc(ROWS * COLS * sizeof(int));
	game.gameEnded = false;

	game.timeStarted = clock();
	game.timeUsed = 0;
	
	initializeBoard(game.board);

	p1.score = 0;
	p2.score = 0;
	p1.name[0] = ' ';
	p2.name[0] = ' ';
	p1.name[1] = '\0';
	p2.name[1] = '\0';

	return game;
}


void initializeBoard(char** board)
{
	for (int i = 0; i < ROWS; ++i)
	{
		board[i] = (char*)malloc(COLS * sizeof(char));

		for (int j = 0; j < COLS; ++j)
			board[i][j] = '-';
	}
}

void flipPlayer(Game* game)
{
	game->p1First = !(game->p1First);
}

void makeMove(Game* game, int col)
{
	// Searching for an empty slot to put the disk in it.
	for (int i = ROWS - 1; i >= 0; i--)
	{
		if (game->board[i][col] == '-')
		{
			if (game->p1First)
			{
				game->board[i][col] = p1.color;  // Updating the board with the player's color.
				updateScore(game, &p1, col, 1);  // Updating the score of the player.
			}
			else
			{
				game->board[i][col] = p2.color;  // Updating the board with the player's color.
				updateScore(game, &p2, col, 1);  // Updating the score of the player.
			}

			flipPlayer(game);
			
			// Handling the redo.
			if (game->totalMoves < game->currMove || game->log[game->currMove] != col)
				game->totalMoves = game->currMove + 1;
			game->log[game->currMove++] = col;  // Adding the current move to the log.

			// Making the computer play his move if the mode of the game was Human vs Computer.
			if (game->ai && !game->p1First)
				makeMove(game, getCol(game->board, ROWS, COLS, col));

			return;
		}
	}
}

void undoMove(Game* game)
{
	if (game->currMove == 0)
		return;

	int c = game->log[--(game->currMove)];

	flipPlayer(game);

	if (game->p1First)
		updateScore(game, &p1, c, -1);
	else
		updateScore(game, &p2, c, -1);

	for (int i = 0; i < ROWS; ++i)
	{
		if (game->board[i][c] != '-')
		{
			game->board[i][c] = '-';
			break;
		}
	}
}

void redoMove(Game* game)
{
	if (game->totalMoves == 0 || game->totalMoves == game->currMove)
		return;

	int c = game->log[game->currMove];
	
	makeMove(game, c);
}

void updateScore(Game* game, Player* player, int c, int increment)
{
	player->score += calcVertically(game, c) * increment;
	player->score += calcHorizontally(game, c) * increment;
	player->score += calcDiagonally(game, c) * increment;
}

// Getting the first row that has free slot in that column.
int getRow(Game* game, int c)
{
	for (int i = 0; i < ROWS; ++i)
		if (game->board[i][c] != '-')
			return i;

	return -1;
}

int calcVertically(Game* game, int c)
{
	int r = getRow(game, c);
	char color = game->board[r][c];

	int connected = 0;
	for (int i = r; i < r + 4 && i < ROWS; ++i)
		if (game->board[i][c] == color)
			connected++;

	return connected == 4;
}

int calcHorizontally(Game* game, int c)
{
	int r = getRow(game, c);
	char color = game->board[r][c];
	int directions[4][2] = { {0, 3}, {1, 2}, {2, 1}, {3, 0} };

	int connected = 0;
	for (int i = 0; i < 4; ++i)
	{
		int left = directions[i][0], right = directions[i][1], currConnected = 1;
		
		for (int x = 1; x <= left && (c - x) >= 0; ++x)
			if (game->board[r][c - x] == color)
				currConnected++;
		
		for (int x = 1; x <= right && (c + x) < COLS; ++x)
			if (game->board[r][c + x] == color)
				currConnected++;

		connected += currConnected == 4;
	}

	return connected;
}

int calcDiagonally(Game* game, int c)
{
	int r = getRow(game, c);
	char color = game->board[r][c];
	int directions[4][2] = { {0, 3}, {1, 2}, {2, 1}, {3, 0} };

	int connected = 0;

	for (int i = 0; i < 4; ++i)
	{
		int left = directions[i][0], right = directions[i][1], currConnected = 1;

		for (int x = 1; x <= left && (c - x) >= 0 && (r - x) >= 0; ++x)
			if (game->board[r - x][c - x] == color)
				currConnected++;

		for (int x = 1; x <= right && (c + x) < COLS && (r + x) < ROWS; ++x)
			if (game->board[r + x][c + x] == color)
				currConnected++;

		connected += currConnected == 4;
	}

	for (int i = 0; i < 4; ++i)
	{
		int left = directions[i][0], right = directions[i][1], currConnected = 1;

		for (int x = 1; x <= left && (c - x) >= 0 && (r + x) < ROWS; ++x)
			if (game->board[r + x][c - x] == color)
				currConnected++;

		for (int x = 1; x <= right && (c + x) < COLS && (r - x) >= 0; ++x)
			if (game->board[r - x][c + x] == color)
				currConnected++;

		connected += currConnected == 4;
	}

	return connected;
}

// Checking if the game has ended.
int endGame(Game* game)
{
	if (game->gameEnded)
		return 4;

	// Return 0 if the game hasn't ended yet.
	if (game->currMove != ROWS * COLS)
		return 0;

	// Return 1 if player 1 won.
	if (p1.score > p2.score)
		return 1;
	// Return 2 if player 1 won.
	else if (p1.score < p2.score)
		return 2;
	// Return 3 if it's draw.
	else
		return 3;
}
