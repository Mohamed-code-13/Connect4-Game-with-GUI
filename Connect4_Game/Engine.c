#include "include/Engine.h"

Player p1 = { 'r', 0 }, p2 = { 'g', 0 };

Game createNewGame()
{
	Game game;

	game.p1First = true;
	game.currMove = 0;
	game.totalMoves = 0;

	game.board = (char**)malloc(ROWS * sizeof(char*));
	game.log = (int*)malloc(ROWS * COLS * sizeof(int));

	initializeBoard(game.board);

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
	for (int i = ROWS - 1; i >= 0; i--)
	{
		if (game->board[i][col] == '-')
		{
			if (game->p1First)
			{
				game->board[i][col] = p1.color;
				updateScore(game, &p1, col, 1);
			}
			else
			{
				game->board[i][col] = p2.color;
				updateScore(game, &p2, col, 1);
			}

			flipPlayer(game);

			if (game->totalMoves < game->currMove || game->log[game->currMove] != col)
				game->totalMoves = game->currMove + 1;
			game->log[game->currMove++] = col;

			endGame(game);

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

void endGame(Game* game)
{
	if (game->currMove != ROWS * COLS)
		return;

	printf("The games has Ended!\n");
	if (p1.score > p2.score)
		printf("Player 1 won! Congratulations\n");
	else if (p1.score < p2.score)
		printf("Player 2 won! Congratulations\n");
	else
		printf("It's a Draw\n");
}
