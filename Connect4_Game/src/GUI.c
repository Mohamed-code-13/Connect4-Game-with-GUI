#include "include/GUI.h"

const char* pathsCirclesImages[] = { "./images/wC.png", "./images/rC.png", "./images/gC.png" };

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Connect 4 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void loadMedia()
{
	for (int i = 0; i < Colors_TOTAL; ++i)
		loadFromFile(&circles[i], pathsCirclesImages[i]);
}

void draw(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	for (int i = 10; i < SCREEN_HEIGHT; i += CIRCLE_SIZE + 20)
	{
		for (int j = 10; j < SCREEN_WIDTH; j += CIRCLE_SIZE + 20)
		{
			enum Colors current;

			switch (game->board[i / 100][j / 100])
			{
			case 'r':
				current = Colors_RED;
				break;
			case 'g':
				current = Colors_GREEN;
				break;
			default:
				current = Colors_WHITE;
				break;
			}

			render(&circles[current], j, i);

			SDL_RenderDrawLine(gRenderer, j - 10, 0, j - 10, SCREEN_HEIGHT);
		}

		SDL_RenderDrawLine(gRenderer, 0, i - 10, SCREEN_WIDTH, i - 10);
	}

	SDL_RenderPresent(gRenderer);
}

void close()
{
	for (int i = 0; i < Colors_TOTAL; ++i)
		freeTexture(&circles[i]);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool loadFromFile(Texture* texture, const char* path)
{
	freeTexture(texture);

	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadSurface = IMG_Load(path);

	if (loadSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	else
	{
		SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 0x00, 0x00, 0x00));

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadSurface);
		if (newTexture == NULL)
			printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		else
		{
			texture->mHeight = loadSurface->h;
			texture->mWidth = loadSurface->w;
		}

		SDL_FreeSurface(loadSurface);
	}

	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

void freeTexture(Texture* tex)
{
	if (tex->mTexture != NULL)
	{
		SDL_DestroyTexture(tex->mTexture);

		tex->mTexture = NULL;
		tex->mHeight = 0;
		tex->mWidth = 0;
	}
}

void render(Texture* tex, int x, int y)
{
	SDL_Rect rect = { x, y, tex->mWidth, tex->mHeight };
	SDL_RenderCopy(gRenderer, tex->mTexture, NULL, &rect);
}