#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include "include/high_score.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int getName_Score_fromFile(char line[], char nameFromFile[])
{
	int i = 0;
	while (line[i] != '/')
		nameFromFile[i] = line[i++];
	nameFromFile[i - 1] = 0;

	char num[5];
	i += 2;
	int j = 0;
	while (line[i])
		num[j++] = line[i++];
	num[j] = 0;
	
	return atoi(num);
}

void formatNewScore(char scoreToSave[], char name[], int score)
{
	char n[5];
	sprintf(n, "%d", score);

	strcat(scoreToSave, name);
	strcat(scoreToSave, " / ");
	strcat(scoreToSave, n);
}

void saveHighScore(char name[], int score, int h)
{
	name++;

	char scoreToSave[60] = {0};
	int indexOfScoreToSave = -1, indexOfSameName = -1;
	formatNewScore(scoreToSave, name, score);

	char** names = (char**)malloc(h * sizeof(char*));
	for (int i = 0; i < h; ++i)
		names[i] = (char*)malloc(60 * sizeof(char));

	int numOfScores = getHighScores(names, h);
	
	for (int i = 0; i < numOfScores; ++i)
	{
		char nameFromFile[50];
		int scoreFromFile = getName_Score_fromFile(names[i], nameFromFile);

		if (score > scoreFromFile)
		{
			if (indexOfScoreToSave == -1)
				indexOfScoreToSave = i;
		}
		if (!strcmp(nameFromFile, name))
		{
			if (indexOfScoreToSave == -1)
				indexOfScoreToSave = -2;

			indexOfSameName = i;
			break;
		}
	}

	FILE* fptr = fopen("./Data/HighScores/highScore.txt", "w");
	if (indexOfScoreToSave == -1)
	{
		fputs(scoreToSave, fptr);
		fputc('\n', fptr);
	}

	else if (indexOfScoreToSave != -2)
	{
		for (int i = 0; i < numOfScores; ++i)
		{
			if (i == indexOfScoreToSave)
			{
				fputs(scoreToSave, fptr);
				fputc('\n', fptr);
			}
			if (i == indexOfSameName)
				continue;
			fputs(names[i], fptr);
			fputc('\n', fptr);
		}
	}

	fclose(fptr);
	for (int i = 0; i < h; ++i)
		free(names[i]);
	free(names);
}

int getHighScores(char** names, int h)
{
	FILE* fptr = fopen("./Data/HighScores/highScore.txt", "r");

	if (fptr == NULL)
	{
		printf("Unable to open the highScore file!\n");
		return -1;
	}

	printf("--------------------------------------\n");
	printf("HighScores:\n");
	int i = 0;
	while (i < h && fgets(names[i], 59, fptr))
	{
		names[i++][strlen(names[i]) - 1] = 0;
		printf("%d - %s\n", i, names[i - 1]);
	}
	printf("--------------------------------------\n");

	fclose(fptr);

	return i;
}
