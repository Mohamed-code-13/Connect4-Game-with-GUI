

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

//#include "include/high_score.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void saveHighScore(char name[], int score)
{
    FILE* fptr = fopen("highScore.txt", "a");

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
    FILE* fptr = fopen("highScore.txt", "r");

    if (fptr == NULL)
    {
        printf("Unable to open the highScore file!\n");
        return -1;
    }

    int i = 0;
    while (i < h && fgets(names[i], 49, fptr))
        names[i++][strlen(names[i]) - 1] = 0;

    fclose(fptr);

    return i;
}



int main()
{
    int SCORES=4;

    char** names = (char**)malloc(SCORES * sizeof(char*));
    for (int i = 0; i < SCORES; ++i)
        names[i] = (char*)malloc(50 * sizeof(char));


    FILE *f1 = fopen("highSCore.txt","r");
    FILE *f2 = fopen("highScore2.txt","w");


    char *playerptr;
    char str[50];
    char currname[50];
    char currscore[4];

    char winnername[]="MO";
    int winnerscore=2;
    while(fgets(str,49,f1))
    {


        playerptr = strtok(str, " ");
        sprintf(currname,"%s",playerptr);

        if(strcmp(winnername,currname)==0)
            printf("%s\n",winnername);
        playerptr = strtok(NULL, " ");
        sprintf(currscore,"%s",playerptr);
            if(atoi(currscore)>winnerscore)
                printf(" greater\n");


        fputs(currname,f2);
        fputc(' ',f2);
        fputs(currscore,f2);


    }

    fclose(f1);
    fclose(f2);
}
