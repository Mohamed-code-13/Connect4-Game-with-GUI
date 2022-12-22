#define _CRT_SECURE_NO_WARNINGS
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
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void render(Texture* tex, int x, int y)
{
	SDL_Rect rect = { x, y, tex->mWidth, tex->mHeight };
	SDL_RenderCopy(gRenderer, tex->mTexture, NULL, &rect);
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

void loadMedia(Game* game)
{
	for (int i = 0; i < Colors_TOTAL; ++i)
		loadFromFile(&circles[i], pathsCirclesImages[i]);

	gFont = TTF_OpenFont("./Data/oswald.ttf", 28);

	renderPlayerText(game);
	renderText();
}

void drawGamePlay(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	for (int i = 60; i < SCREEN_HEIGHT - 50; i += CIRCLE_SIZE + 10)
	{
		for (int j = 10; j < SCREEN_WIDTH; j += CIRCLE_SIZE + 10)
		{
			enum Colors current;

			switch (game->board[(i - 50) / 80][j / 80])
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

			SDL_RenderDrawLine(gRenderer, j - 10, 60, j - 10, SCREEN_HEIGHT - 50);
		}

		SDL_RenderDrawLine(gRenderer, 0, i, SCREEN_WIDTH, i);
	}

	SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, SCREEN_HEIGHT - 50);

	render(&player_1_text, 20, (SCREEN_HEIGHT - 50));
	render(&player_2_text, 20, 10);

	SDL_RenderPresent(gRenderer);
}

void drawMainMenu(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0xCA, 0x2C, 0x40, 0xFF);
	SDL_RenderFillRect(gRenderer, &buttons[0].rect);
	render(&buttons[0].texture, buttons[0].rect.x + 40, buttons[0].rect.y);

	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = 1; i < BUTTONS_TotalMainMenu; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void drawModeOptions(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = BUTTONS_HvH; i < BUTTONS_TotalMode; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void drawLoadGame(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = BUTTONS_game1; i < BUTTONS_TOTAL; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void draw(Game* game, enum State st)
{
	SDL_SetRenderDrawColor(gRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	switch (st)
	{
	case State_GamePlay:
		drawGamePlay(game);
		break;
	case State_MainMenu:
		drawMainMenu(game);
		break;
	case State_ModeOptions:
		drawModeOptions(game);
		break;
	case State_LoadGame:
		drawLoadGame(game);
		break;
	default:
		break;
	}
}


bool loadFromRenderedText(Texture* texture, const char* textureText, SDL_Color textColor)
{
	freeTexture(texture);

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture->mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture->mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			texture->mWidth = textSurface->w;
			texture->mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	return texture->mTexture != NULL;
}

void getText(char* txt, Player* p, bool p1First)
{
	char sc[15];
	sprintf(sc, "%d", p->score);

	char c[7];
	if (p->color == 'r')
		strcpy(c, "RED");
	else
		strcpy(c, "GREEN");

	strcpy(txt, c);
	strcat(txt, " Player   Score: ");
	strcat(txt, sc);
	
	if (p1First)
		strcat(txt, "  (Your turn)");
}

void renderPlayerText(Game* game)
{
	char* p_text = (char*)malloc(50 * sizeof(char));

	SDL_Color textColor = { 255, 0, 0 };
	getText(p_text, &p1, game->p1First);
	loadFromRenderedText(&player_1_text, p_text, textColor);

	SDL_Color textColor2 = { 0, 255, 0 };
	getText(p_text, &p2, !(game->p1First));
	loadFromRenderedText(&player_2_text, p_text, textColor2);

	free(p_text);
}

void mainMenuText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	loadFromRenderedText(&buttons[0].texture, opts[0], textColor);
	strcpy(buttons[0].name, opts[0]);

	buttons[0].rect.x = SCREEN_WIDTH / 2 - 220;
	buttons[0].rect.y = SCREEN_HEIGHT / 9.0;
	buttons[0].rect.w = 480;
	buttons[0].rect.h = 50;

	for (int i = BUTTONS_newGame; i < BUTTONS_TotalMainMenu; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);
		strcpy(buttons[i].name, opts[i]);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 80;
		buttons[i].rect.y = SCREEN_HEIGHT * (i / 9.0 + 2 / 9.0);
		buttons[i].rect.w = 200;
		buttons[i].rect.h = 50;
	}
}

void modeOptionsText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	for (int i = BUTTONS_HvH; i < BUTTONS_TotalMode; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);
		strcpy(buttons[i].name, opts[i]);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 130;
		buttons[i].rect.y = SCREEN_HEIGHT * ((i - 1) / 9.0);
		buttons[i].rect.w = 280;
		buttons[i].rect.h = 50;
	}
}

void loadGameText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	for (int i = BUTTONS_game1; i < BUTTONS_TOTAL; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);
		strcpy(buttons[i].name, opts[i]);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 120;
		buttons[i].rect.y = SCREEN_HEIGHT * ((i - 4) / 9.0);
		buttons[i].rect.w = 250;
		buttons[i].rect.h = 50;
	}
}

void renderText()
{
	const char* opts[] = {"Welcome to Connect4 Game Main Menu", "New Game", "Load Game", "HighScore", "Quit",
							"Human vs Human", "Human vs Computer", "Game 1 (Latest)", "Game 2", "Game 3 (Oldest)"};

	mainMenuText(opts);
	modeOptionsText(opts);
	loadGameText(opts);
}

void close(Game* game)
{
	saveGame(game);

	for (int i = 0; i < ROWS; ++i)
		free(game->board[i]);
	free(game->board);
	free(game->log);

	freeTexture(&player_1_text);
	freeTexture(&player_2_text);

	for (int i = 0; i < Colors_TOTAL; ++i)
		freeTexture(&circles[i]);

	for (int i = 0; i < BUTTONS_TOTAL; ++i)
		freeTexture(&buttons[i]);

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
