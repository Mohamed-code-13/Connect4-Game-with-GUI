#define _CRT_SECURE_NO_WARNINGS

#include "include/save_load_game.h"

void saveGame(Game* game)
{
	FILE* fptr = fopen("game1.dat", "wb");
	if (!fptr)
	{
		printf("Error\n");
		exit(1);
	}
	for (int i = 0; i < ROWS; ++i)
	{
		fwrite(game->board[i], sizeof(game->board[i]), 1, fptr);
	}
	// fwrite(game, sizeof(Game), 1, fptr);

	fclose(fptr);
}

void loadGame(Game* game)
{
	FILE* fptr = fopen("game1.dat", "rb");
	if (!fptr)
	{
		return;
	}

	for (int i = 0; i < ROWS; ++i)
	{
		fread(game->board[i], sizeof(game->board[i]), 1, fptr);
	}
	// fread(game, sizeof(Game), 1, fptr);

	fclose(fptr);
}
