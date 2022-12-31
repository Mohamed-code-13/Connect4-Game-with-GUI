#define _CRT_SECURE_NO_WARNINGS
#include "include/GUI.h"

const char* pathsCirclesImages[] = { "./images/wC.png", "./images/rC.png", "./images/gC.png" };

Texture circles[Colors_TOTAL];
Texture player_1_text, player_2_text;
Button gameEnd[10];
Button scores[200];
int numOfScores;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
TTF_Font* gFont;

// Initializing the SDL and creating a new window.
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

// Loading Images to the game.
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
			// Getting the new dimensions.
			texture->mHeight = loadSurface->h;
			texture->mWidth = loadSurface->w;
		}

		// Freeing old surface.
		SDL_FreeSurface(loadSurface);
	}

	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

void loadMedia(Game* game)
{
	for (int i = 0; i < Colors_TOTAL; ++i)
		loadFromFile(&circles[i], pathsCirclesImages[i]);

	// Initializing the font.
	gFont = TTF_OpenFont("./Data/oswald.ttf", 28);

	renderPlayerText(game);
	renderText();
}

// Drawing the board with each player score and who has to play.
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

void drawMainMenu()
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

void drawModeOptions()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = BUTTONS_HvH; i < BUTTONS_TotalMode; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void drawLoadGame()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFC, 0xB7, 0x50, 0xFF);
	for (int i = BUTTONS_game1; i < BUTTONS_TOTAL; ++i)
	{
		SDL_RenderFillRect(gRenderer, &buttons[i].rect);

		render(&buttons[i].texture, buttons[i].rect.x + 40, buttons[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void drawHighScore()
{
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	for (int i = 0; i < numOfScores; ++i)
	{

		SDL_RenderFillRect(gRenderer, &(scores[i].rect));

		render(&(scores[i].texture), scores[i].rect.x + 40, scores[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void drawTime()
{
	SDL_RenderFillRect(gRenderer, &gameEnd[9].rect);
	render(&gameEnd[9].texture, gameEnd[9].rect.x + 20, gameEnd[9].rect.y);
}

void drawWinnerName(Player* p)
{
	if (p->color == 'r')
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	else
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);

	for (int i = 0; i < 3; ++i)
	{
		SDL_RenderFillRect(gRenderer, &gameEnd[i].rect);

		render(&gameEnd[i].texture, gameEnd[i].rect.x + 40, gameEnd[i].rect.y);
	}
	drawTime();

	SDL_RenderPresent(gRenderer);
}

void drawDraw()
{
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);

	for (int i = 3; i < 5; ++i)
	{
		SDL_RenderFillRect(gRenderer, &gameEnd[i].rect);
		render(&gameEnd[i].texture, gameEnd[i].rect.x + 20, gameEnd[i].rect.y);
	}
	drawTime();

	SDL_RenderPresent(gRenderer);
}

void drawComputerWon()
{
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);

	for (int i = 5; i < 7; ++i)
	{
		SDL_RenderFillRect(gRenderer, &gameEnd[i].rect);
		render(&gameEnd[i].texture, gameEnd[i].rect.x + 20, gameEnd[i].rect.y);
	}
	drawTime();

	SDL_RenderPresent(gRenderer);
}

void drawLoadFailed()
{
	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);

	for (int i = 7; i < 9; ++i)
	{
		SDL_RenderFillRect(gRenderer, &gameEnd[i].rect);
		render(&gameEnd[i].texture, gameEnd[i].rect.x + 20, gameEnd[i].rect.y);
	}

	SDL_RenderPresent(gRenderer);
}

void draw(Game* game, enum State st, Player* p)
{
	SDL_SetRenderDrawColor(gRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	switch (st)
	{
	case State_GamePlay:
		drawGamePlay(game);
		break;
	case State_MainMenu:
		drawMainMenu();
		break;
	case State_ModeOptions:
		drawModeOptions();
		break;
	case State_LoadGame:
		drawLoadGame();
		break;
	case State_HighScore:
		drawHighScore();
		break;
	case State_WinnerName:
		drawWinnerName(p);
		break;
	case State_Draw:
		drawDraw();
		break;
	case State_ComputerWon:
		drawComputerWon();
		break;
	case State_LoadFailed:
		drawLoadFailed();
		break;
	default:
		break;
	}
}

// Loading text to the game window.
bool loadFromRenderedText(Texture* texture, const char* textureText, SDL_Color textColor)
{
	freeTexture(texture);

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);

	if (textSurface == NULL)
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
	{
		texture->mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture->mTexture == NULL)
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		else
		{
			// Getting the new dimensions.
			texture->mWidth = textSurface->w;
			texture->mHeight = textSurface->h;
		}

		// Freeing old surface.
		SDL_FreeSurface(textSurface);
	}

	return texture->mTexture != NULL;
}

// Text for each player to display in the game play.
// ex. RED Player Score: 0  (Your turn).
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

	SDL_Color textColor_red = { 255, 0, 0 };
	getText(p_text, &p1, game->p1First);
	loadFromRenderedText(&player_1_text, p_text, textColor_red);

	SDL_Color textColor_green = { 0, 255, 0 };
	getText(p_text, &p2, !(game->p1First));
	loadFromRenderedText(&player_2_text, p_text, textColor_green);

	free(p_text);
}

// Initializing main menu text.
void mainMenuText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	loadFromRenderedText(&buttons[0].texture, opts[0], textColor);

	buttons[0].rect.x = SCREEN_WIDTH / 2 - 220;
	buttons[0].rect.y = SCREEN_HEIGHT / 9.0;
	buttons[0].rect.w = 480;
	buttons[0].rect.h = 50;

	for (int i = BUTTONS_newGame; i < BUTTONS_TotalMainMenu; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 80;
		buttons[i].rect.y = SCREEN_HEIGHT * (i / 9.0 + 2 / 9.0);
		buttons[i].rect.w = 200;
		buttons[i].rect.h = 50;
	}
}

// Initializing mode optings text.
void modeOptionsText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	for (int i = BUTTONS_HvH; i < BUTTONS_TotalMode; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 130;
		buttons[i].rect.y = SCREEN_HEIGHT * ((i - 2) / 9.0);
		buttons[i].rect.w = 280;
		buttons[i].rect.h = 50;
	}
}

// Initializing load game text.
void loadGameText(const char* opts[])
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	for (int i = BUTTONS_game1; i < BUTTONS_TOTAL; ++i)
	{
		loadFromRenderedText(&buttons[i].texture, opts[i], textColor);

		buttons[i].rect.x = SCREEN_WIDTH / 2 - 120;
		buttons[i].rect.y = SCREEN_HEIGHT * ((i - 5) / 9.0);
		buttons[i].rect.w = 250;
		buttons[i].rect.h = 50;
	}
}

// Initializing high score board text.
void highScoreText()
{
	char** names = (char**)malloc(SCORES * sizeof(char*));
	for (int i = 0; i < SCORES; ++i)
		names[i] = (char*)malloc(60 * sizeof(char));

	numOfScores = getHighScores(names, SCORES);
	if (numOfScores == -1)
	{
		printf("Coulding open the highScore file!\n");
		return;
	}

	SDL_Color textColor = { 0x00, 0x00, 0x00 };
	
	int r = 0, c = 0;
	for (int i = 0; i < numOfScores; ++i)
	{
		loadFromRenderedText(&scores[i].texture, names[i], textColor);

		if (SCREEN_HEIGHT * c / 9 >= SCREEN_HEIGHT)
		{
			r = 1;
			c = 0;
		}
		scores[i].rect.x = 20 + r * SCREEN_WIDTH / 2;
		scores[i].rect.y = SCREEN_HEIGHT * c++ / 9 + 10;
		scores[i].rect.w = 250;
		scores[i].rect.h = 50;
	}
	
	for (int i = 0; i < SCORES; ++i)
		free(names[i]);
	free(names);
}

// Initializing end game page text when one player won.
void endGameText(Player* p)
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	const char* end = "Enter your Name (max 50): ";
	const char* winnerMsg;

	if (p->color == 'r')
		winnerMsg = "Player (RED) won. Congraturlations!";
	else
		winnerMsg = "Player (GREEN) won. Congraturlations!";

	loadFromRenderedText(&gameEnd[0].texture, winnerMsg, textColor);
	loadFromRenderedText(&gameEnd[1].texture, end, textColor);
	loadFromRenderedText(&gameEnd[2].texture, p->name, textColor);

	gameEnd[0].rect.x = SCREEN_WIDTH / 2 - 200;
	gameEnd[0].rect.y = SCREEN_HEIGHT / 9;
	gameEnd[0].rect.w = 470;
	gameEnd[0].rect.h = 50;

	for (int i = 1; i < 3; ++i)
	{
		gameEnd[i].rect.x = SCREEN_WIDTH / 2 - 160;
		gameEnd[i].rect.y = SCREEN_HEIGHT / 9 * ( i + 3 );
		gameEnd[i].rect.w = 350;
		gameEnd[i].rect.h = 50;
	}
}

// Initializing end game page text when it's draw.
void endGameDrawText()
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	const char* msgs[] = { "It's Draw!", "Press Enter to return to Main Menu" };

	loadFromRenderedText(&gameEnd[3].texture, msgs[0], textColor);
	loadFromRenderedText(&gameEnd[4].texture, msgs[1], textColor);

	gameEnd[3].rect.x = SCREEN_WIDTH / 2 - 50;
	gameEnd[3].rect.y = SCREEN_HEIGHT / 9;
	gameEnd[3].rect.w = 130;
	gameEnd[3].rect.h = 50;

	gameEnd[4].rect.x = SCREEN_WIDTH / 2 - 200;
	gameEnd[4].rect.y = SCREEN_HEIGHT / 9 * 6;
	gameEnd[4].rect.w = 400;
	gameEnd[4].rect.h = 50;
}

// Initializing end game page text when the computer won.
void computerWonDraw()
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	const char* msgs[] = { "Computer Won!", "Press Enter to return to Main Menu" };

	loadFromRenderedText(&gameEnd[5].texture, msgs[0], textColor);
	loadFromRenderedText(&gameEnd[6].texture, msgs[1], textColor);

	gameEnd[5].rect.x = SCREEN_WIDTH / 2 - 80;
	gameEnd[5].rect.y = SCREEN_HEIGHT / 9;
	gameEnd[5].rect.w = 200;
	gameEnd[5].rect.h = 50;

	gameEnd[6].rect.x = SCREEN_WIDTH / 2 - 200;
	gameEnd[6].rect.y = SCREEN_HEIGHT / 9 * 5;
	gameEnd[6].rect.w = 400;
	gameEnd[6].rect.h = 50;
}

// Initializing load failed page text.
void loadFailedText()
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	const char* msgs[] = { "Failed to Load the game!", "Press Enter to return to Main Menu" };

	loadFromRenderedText(&gameEnd[7].texture, msgs[0], textColor);
	loadFromRenderedText(&gameEnd[8].texture, msgs[1], textColor);

	gameEnd[7].rect.x = SCREEN_WIDTH / 2 - 120;
	gameEnd[7].rect.y = SCREEN_HEIGHT / 9;
	gameEnd[7].rect.w = 280;
	gameEnd[7].rect.h = 50;

	gameEnd[8].rect.x = SCREEN_WIDTH / 2 - 200;
	gameEnd[8].rect.y = SCREEN_HEIGHT / 9 * 5;
	gameEnd[8].rect.w = 400;
	gameEnd[8].rect.h = 50;
}

// Calculating the time of the game and displaying it.
void getTime(Game* game, char timeTook[70])
{
	game->timeUsed += ((int)(game->timeEnded - game->timeStarted)) / CLOCKS_PER_SEC;

	int mins = game->timeUsed / 60;
	int seconds = game->timeUsed % 60;

	char mins_string[5];
	char seconds_string[5];

	// Converting int time to string.
	sprintf(mins_string, "%d", mins);
	sprintf(seconds_string, "%d", seconds);

	// Concatenating the mins with the original string.
	strcat(timeTook, mins_string);
	strcat(timeTook, " mins and ");

	// Concatenating the seconds with the original string.
	strcat(timeTook, seconds_string);
	strcat(timeTook, " seconds.");
}

void timeText(Game* game)
{
	SDL_Color textColor = { 0x00, 0x00, 0x00 };

	char timeTook[50] = "Time took: ";
	getTime(game, timeTook);

	loadFromRenderedText(&gameEnd[9].texture, timeTook, textColor);

	gameEnd[9].rect.x = SCREEN_WIDTH / 2 - 200;
	gameEnd[9].rect.y = SCREEN_HEIGHT / 9 * 3;
	gameEnd[9].rect.w = 400;
	gameEnd[9].rect.h = 50;
}

void renderText()
{
	const char* opts[] = {"Welcome to Connect4 Game Main Menu", "New Game", "Resume", "Load Game", "HighScore", "Quit",
							"Human vs Human", "Human vs Computer", "Game 1 (Latest)", "Game 2", "Game 3 (Oldest)"};

	mainMenuText(opts);
	modeOptionsText(opts);
	loadGameText(opts);
	highScoreText();
	endGameText(&p1);
	endGameDrawText();
	computerWonDraw();
	loadFailedText();
}

// Freeing all the data the program took.
// Saving the game before closing.
void close(Game* game)
{
	if (!game->gameEnded && game->totalMoves != 0)
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
		freeTexture(&buttons[i].texture);

	for (int i = 0; i < numOfScores; ++i)
		freeTexture(&scores[i].texture);

	for (int i = 0; i < 3; ++i)
		freeTexture(&gameEnd[i].texture);

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
