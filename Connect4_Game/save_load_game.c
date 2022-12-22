#define _CRT_SECURE_NO_WARNINGS

#include "include/save_load_game.h"

void saveGame(Game* game,int n)
{
    int size = ROWS*COLS;
    FILE* fptr = fopen("game1.dat", "rb+");
    if (!fptr)
    {
        printf("Error\n");
        exit(1);
    }



    char *buff = malloc((size)*sizeof(char));
    buff[size]='\0';

    fseek(fptr,size*(n),SEEK_SET);
    for (int i = 0; i < ROWS; ++i)
    {
        fread(buff+i*COLS, (COLS) * sizeof(char), 1, fptr);

    }

    printf("%s",buff);

    fseek(fptr,size*(n-1),SEEK_SET);
    for (int i = 0; i < ROWS; ++i)
    {
        fwrite(buff+i*COLS, (COLS) * sizeof(char), 1, fptr);

    }
    free(buff);
    fclose(fptr);


}
void updateLast(Game *game){


{
    int size = ROWS*COLS;
    FILE* fptr = fopen("game1.dat", "rb+");
    if (!fptr)
    {
        printf("Error\n");
        exit(1);
    }




    fseek(fptr,size*(3-1),SEEK_SET);
    for (int i = 0; i < ROWS; ++i)
    {
        fwrite(game->board[i], (COLS) * sizeof(char), 1, fptr);

    }
    fclose(fptr);


}






}


void loadGame(Game* game,int GameNo)
{
    FILE* fptr = fopen("game1.dat", "rb");
    if (!fptr)
    {
        return;
    }
    fseek(fptr, (ROWS*COLS)*(GameNo-1),SEEK_SET);
    for (int i = 0; i < ROWS; ++i)
    {

        fread(game->board[i], (COLS) * sizeof(char), 1, fptr);
    }


    fclose(fptr);
}
