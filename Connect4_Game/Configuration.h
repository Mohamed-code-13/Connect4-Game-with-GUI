
#include <stdbool.h>

static char currPath[51] = "./config.txt";

typedef struct
{
	int height;
	int width;
	int highScore;
} Configuration;

Configuration config;



 bool  readConfig(char *currPath);
 void setConfig();
