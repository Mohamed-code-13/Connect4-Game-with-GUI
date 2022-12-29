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
	State_ComputerWon,
	State_Draw,
	State_LoadFailed,
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

Button buttons[BUTTONS_TOTAL];

bool init();
void loadMedia(Game* game);
void draw(Game* game, enum State st, Player* p);
void renderText();
void renderPlayerText(Game* game);
void close(Game* game);
void endGameText(Player* p);
void highScoreText();