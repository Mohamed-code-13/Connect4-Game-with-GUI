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

void loadMedia(Game* game)
{
	for (int i = 0; i < Colors_TOTAL; ++i)
		loadFromFile(&circles[i], pathsCirclesImages[i]);

	gFont = TTF_OpenFont("acme.ttf", 28);

	renderPlayerText(game);
	renderMainMenuText();
}

void draw(Game* game, enum State st)
{
	if (st == State_GamePlay)
		drawGamePlay(game);
	else if (st == State_MainMenu)
		drawMainMenu(game);
}

void drawGamePlay(Game* game)
{
	SDL_SetRenderDrawColor(gRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
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
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	SDL_SetRenderDrawColor(gRenderer, 0xCA, 0x2C, 0x40, 0xFF);
	SDL_RenderFillRect(gRenderer, &buttons[0].rect);
	render(&buttons[0].texture, buttons[0].rect.x + 40, buttons[0].rect.y);

	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = 1; i < BUTTONS_Total; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void close(Game* game)
{


	for (int i = 0; i < ROWS; ++i)
		free(game->board[i]);
	free(game->board);
	free(game->log);

	freeTexture(&player_1_text);
	freeTexture(&player_2_text);

	for (int i = 0; i < Colors_TOTAL; ++i)
		freeTexture(&circles[i]);

	for (int i = 0; i < BUTTONS_Total; ++i)
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
	getText(p_text, &p2, !game->p1First);
	loadFromRenderedText(&player_2_text, p_text, textColor2);

	free(p_text);
}


void renderMainMenuText()
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	const char* opts[] = {"Welcome to Connect4 Game Main Menu", "New Game", "Load Game", "HighScore", "Quit"};

	loadFromRenderedText(&buttons[0].texture, opts[0], textColor);
	strcpy(buttons[0].name, opts[0]);

	buttons[0].rect.x = SCREEN_WIDTH / 2 - 220;
	buttons[0].rect.y = SCREEN_HEIGHT / 9.0;
	buttons[0].rect.w = 480;
	buttons[0].rect.h = 50;

	for (int i = 1; i < 5; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);
		strcpy(buttons[i].name, opts[i]);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 80;
		buttons[i].rect.y = SCREEN_HEIGHT * (i / 9.0 + 2 / 9.0);
		buttons[i].rect.w = 200;
		buttons[i].rect.h = 50;
	}
}

void handleMouseGamePlay(Game* game, int x, int y)
{
	int row = (y - (CIRCLE_SIZE + 10)) / (CIRCLE_SIZE + 10), col = x / (CIRCLE_SIZE + 10);

	printf("row: %d  col: %d\n", row, col);

	makeMove(game, col);

	renderPlayerText(game);

	printf("Score player1: %d\n", p1.score);
	printf("Score player2: %d\n", p2.score);
}

void handleMouseMainMenu(int x, int y, bool* quit, enum State* currentState)
{
	for (int i = 1; i < BUTTONS_Total; ++i)
	{
		int x1 = buttons[i].rect.x, x2 = buttons[i].rect.x + buttons[i].rect.w;
		int y1 = buttons[i].rect.y, y2 = buttons[i].rect.y + buttons[i].rect.h;

		if (x1 <= x && x2 >= x && y1 <= y && y2 >= y)
		{
			switch (i)
			{
			case BUTTONS_newGame:
				*currentState = State_GamePlay;
				break;
			case BUTTONS_loadGame:
				printf("%s\n", buttons[i].name);
				break;
			case BUTTONS_highScore:
				printf("%s\n", buttons[i].name);
				break;
			case BUTTONS_quit:
				*quit = true;
				break;
			default:
				break;
			}
		}
	}
}
