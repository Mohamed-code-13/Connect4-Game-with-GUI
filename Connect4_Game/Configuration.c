#include "include/Configuration.h"
#define MAX_DIGITS 2
#include <ctype.h>
#include <stdio.h>
#include <string.h>


Configuration config;
bool  readConfig(char *currPath)
{
    char NumString[3][MAX_DIGITS+1]={"","",""};
    int dim=0,dig=0;
    FILE *f = fopen(currPath, "r");

    char sconfig[]=
        "#<configurations>\
#<height>#?#</height>\
#<width>#?#</width>\
#<highscores>#?#</highscores>\
#</configurations>#";

    char* confptr = sconfig;
    int c = getc(f);
    int quit = 0;

    while (!quit )
    {
        while((*confptr)=='?'&&c != EOF)
        {
            if(isdigit(c))
            {
                dig=0;

                while (isdigit(c))
                {
                    if(dig<MAX_DIGITS)
                    {
                        NumString[dim][dig++]= c;
                        c= getc(f);
                    }
                    else
                    {
                        confptr++;
                        quit=1;
                        break;
                    }
                }
                dim++;
            }

            if(dig<1)
                quit=1;

            confptr++;
            dig=0;
            break;
        }

        ////////////////////////
        while((*confptr)=='#'&&c != EOF)
        {
            if(!(isspace(c)))
            {
                confptr++;
                break;
            }
            c= getc(f);
        }
        ///////////////////////////

        if(*(confptr)=='?') {}
        else if(tolower(c)==  *(confptr))
        {
            confptr++;
            c = getc(f);
        }
        else
        {
            quit=1;
        }


    }
    fclose(f);

    if (confptr==(sconfig+strlen(sconfig))-1)
    {
        printf("successFormant");
        config.height=atoi(NumString[0]);
        config.width=atoi(NumString[1]);
        config.highScore=atoi(NumString[2]);
        return (config.height<50&&config.width<50);
    }
    else
        printf("invalid");
    return false;
}



void setConfig()
{
    if(readConfig(currPath))
        return 1;
    for(int n=1; n<=3 ; n++)
    {

        printf("ENTER VALID CONFIGUATIONS FILE PATH\n");
        scanf("%50s",&currPath);
        if(readConfig(currPath))
            return;
    }
    config.height=7;
    config.width=9;
    config.highScore=10;
}
