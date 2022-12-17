#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
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

SDL_Window* gWindow;
SDL_Renderer* gRenderer;


bool loadFromFile(Texture* texture, const char* path);
void freeTexture(Texture* tex);
void render(Texture* tex, int x, int y);

bool init();
void loadMedia();
void draw(Game* game);
void close();

