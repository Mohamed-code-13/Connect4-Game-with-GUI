#include "include/high_score.h"
#include <stdio.h>
#include <string.h>

int getHighScores(char names[2][50], int h)
{
    FILE* fptr = fopen("./Data/HighScores/highScores.txt", "r");

    if (fptr == NULL)
        return;

    char ch;
    int i = 0, j = 0;
    while (ch != EOF && i < h)
    {
        ch = fgetc(fptr);

        if (ch == '\n')
        {
            names[i][j] = '\0';

            i++;
            j = 0;
        }
        else
            names[i][j++]=ch;
    }

    fclose(fptr);

    return i;
}
