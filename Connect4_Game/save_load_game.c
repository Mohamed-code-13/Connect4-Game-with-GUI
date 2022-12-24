#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "include/save_load_game.h"

void saveGame(Game* game)
{
	rename("./Data/saved_game/game3.bin", "./Data/saved_game/game4.bin");
	rename("./Data/saved_game/game2.bin", "./Data/saved_game/game3.bin");
	rename("./Data/saved_game/game1.bin", "./Data/saved_game/game2.bin");
	rename("./Data/saved_game/game4.bin", "./Data/saved_game/game1.bin");


	FILE* fptr = fopen("./Data/saved_game/game1.bin", "wb");
	if (!fptr)
	{
		printf("Error\n");
		exit(1);
	}
	for (int i = 0; i < ROWS; ++i)
		fwrite(game->board[i], COLS * sizeof(char), 1, fptr);

	fwrite(game->log, ROWS * COLS * sizeof(int), 1, fptr);

	putw(game->currMove, fptr);
	putw(game->totalMoves, fptr);
	fwrite(&(game->p1First), sizeof(bool), 1, fptr);

	putw(p1.score, fptr);
	putw(p2.score, fptr);

	fclose(fptr);
}

void loadGame(Game* game, int n)
{
	char* num = "1.bin";

	switch (n)
	{
	case 1:
		num = "1.bin";
		break;
	case 2:
		num = "2.bin";
		break;
	case 3:
		num = "3.bin";
		break;
	default:
		break;
	}

	char filePath[30] = "./Data/saved_game/game";
	strcat(filePath, num);

	FILE* fptr = fopen(filePath, "rb");
	if (!fptr)
		return;

	for (int i = 0; i < ROWS; ++i)
		fread(game->board[i], COLS * sizeof(char), 1, fptr);

	fread(game->log, ROWS * COLS * sizeof(int), 1, fptr);

	game->currMove = getw(fptr);
	game->totalMoves = getw(fptr);
	fread(&(game->p1First), sizeof(bool), 1, fptr);

	p1.score = getw(fptr);
	p2.score = getw(fptr);

	fclose(fptr);
}
