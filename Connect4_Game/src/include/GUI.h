
#include <string.h>
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
	State_Total
};

enum BUTTONS
{
	BUTTONS_CONNECT4,
	BUTTONS_newGame,
	BUTTONS_loadGame,
	BUTTONS_highScore,
	BUTTONS_quit,
	BUTTONS_Total
};

Texture circles[Colors_TOTAL];
Texture player_1_text, player_2_text;
Button buttons[BUTTONS_Total];

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
TTF_Font* gFont;


bool loadFromFile(Texture* texture, const char* path);
bool loadFromRenderedText(Texture* texture, const char* textureText, SDL_Color textColor);
void freeTexture(Texture* tex);
void render(Texture* tex, int x, int y);
void renderPlayerText(Game* game);
void renderMainMenuText();
void getText(char* txt, Player* p, bool p1First);

bool init();
void loadMedia(Game* game);
void draw(Game* game, enum State st);
void drawGamePlay(Game* game);
void drawMainMenu(Game* game);
void close(Game* game);
void handleMouseGamePlay(Game* game, int x, int y);
void handleMouseMainMenu(int x, int y, bool* quit, enum State* currentState);
