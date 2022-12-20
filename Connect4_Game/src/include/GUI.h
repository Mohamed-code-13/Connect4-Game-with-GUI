#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "include/Engine.h"


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

Texture circles[Colors_TOTAL];
Texture player_1_text, player_2_text;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
TTF_Font* gFont;


bool loadFromFile(Texture* texture, const char* path);
bool loadFromRenderedText(Texture* texture, const char* textureText, SDL_Color textColor);
void freeTexture(Texture* tex);
void render(Texture* tex, int x, int y);

bool init();
void loadMedia();
void draw(Game* game);
void close();
void getText(char* txt, Player* p);
void renderText();

