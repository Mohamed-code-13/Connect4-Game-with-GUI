#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "include/high_score.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void saveHighScore(char name[], int score)
{
	FILE* fptr = fopen("./Data/HighScores/highScore.txt", "a");

	if (fptr == NULL)
	{
		printf("The file can't be opened!\n");
		return;
	}

	char sc[10];
	sprintf(sc, "%d", score);

	fputs(name, fptr);
	fputc(' ', fptr);
	fputs(sc, fptr);
	fputc('\n', fptr);

	fclose(fptr);
}

int getHighScores(char** names, int h)
{
	FILE* fptr = fopen("./Data/HighScores/highScore.txt", "r");

	if (fptr == NULL)
	{
		printf("Unable to open the file!\n");
		return -1;
	}

	int i = 0;
	while (i < h && fgets(names[i], 49, fptr))
		names[i++][strlen(names[i]) - 1] = 0;

	fclose(fptr);

	return i;
}
