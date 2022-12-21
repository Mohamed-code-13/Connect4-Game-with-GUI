

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Engine.h"


typedef struct
{
	SDL_Texture* mTexture;

	int mWidth;
	int mHeight;
} Texture;

enum Colors
{
	Colors_WHITE,
	Colors_RED,
	Colors_GREEN,
	Colors_TOTAL
};

typedef struct
{
    SDL_Texture* mTexture;
    char *label;
}  Button;




Texture circles[Colors_TOTAL];


SDL_Window* gWindow;
SDL_Window* mWindow;

SDL_Renderer* gRenderer;
SDL_Renderer* mRenderer;

TTF_Font* gFont;

SDL_Surface *msurface ;
SDL_Texture *mtexture ;
SDL_Texture *stexture ;
SDL_Texture *ltexture ;
SDL_Texture *ttexture ;
SDL_Texture *qtexture ;
SDL_Texture *htexture ;
SDL_Texture *ctexture ;


Button StartButton;
Button LoadButton;
Button TopScoreButton;
Button QuitButton;

Button vsHumanButton;
Button vsCompButton;



bool loadFromFile(Texture* texture, const char* path);
void freeTexture(Texture* tex);
void render(Texture* tex, int x, int y);
void Mrender(Texture* tex, int x, int y);


bool init();
void loadMedia();
void draw(Game* game);
void writeButton(Button* B);
void createButtons();
void renderMainMenu();
void renderChooseMode();
void close();

