#include "GUI.h"

void runGame();

void handleMouseGamePlay(Game* game, int x, int y);
void handleMouseMainMenu(int x, int y, bool* quit, enum State* currentState);
void handleMouseModeOptions(int x, int y, Game* game, enum State* currentState);
void handleMouseLoadGame(int x, int y, Game* game, enum State* currentState);
void handleTyping(SDL_Event* e, bool* typing, Player* winner, Game* game, enum State* currentState);
