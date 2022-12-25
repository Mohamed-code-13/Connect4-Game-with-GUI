#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "save_load_game.h"

typedef struct
{
	SDL_Texture* mTexture;

	int mWidth;
	int mHeight;
} Texture;

typedef struct
{
	Texture texture;
	SDL_Rect rect;
	char name[50];
} Button;

enum Colors
{
	Colors_WHITE,
	Colors_RED,
	Colors_GREEN,
	Colors_TOTAL
};

enum State
{
	State_GamePlay,
	State_MainMenu,
	State_ModeOptions,
	State_LoadGame,
	State_HighScore,
	State_WinnerName,
	State_Total
};

enum BUTTONS
{
	BUTTONS_CONNECT4,
	BUTTONS_newGame,
	BUTTONS_resume,
	BUTTONS_loadGame,
	BUTTONS_highScore,
	BUTTONS_quit,
	BUTTONS_HvH,
	BUTTONS_HvC,
	BUTTONS_game1,
	BUTTONS_game2,
	BUTTONS_game3,
	BUTTONS_TOTAL,
	BUTTONS_TotalMainMenu = 6,
	BUTTONS_TotalMode = 8,
	BUTTONS_TotalSavedGames = 11,
};

Texture circles[Colors_TOTAL];
Texture player_1_text, player_2_text;
Button buttons[BUTTONS_TOTAL];
Button gameEnd[2];

Button scores[200];
int numOfScores;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
TTF_Font* gFont;


bool init();
void loadMedia(Game* game);
void draw(Game* game, enum State st);
void renderText();
void renderPlayerText(Game* game);
void close(Game* game);
void endGameText(Player* p);
void highScoreText();