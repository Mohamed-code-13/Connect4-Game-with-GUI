#define _CRT_SECURE_NO_WARNINGS

#include "include/Game.h"


void runGame()
{
	Game game = createNewGame();
	enum State currentState = State_MainMenu;

	if (!init())
		printf("Failed to initialize!\n");
	else
	{
		loadMedia(&game);
		
		bool quit = false, typing = false;
		SDL_Event e;

		int x, y;
		while (!quit)
		{
			while (SDL_WaitEvent(&e))
			{
				if (e.type == SDL_QUIT || quit)
				{
					quit = true;
					break;
				}

				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					SDL_GetMouseState(&x, &y);
					
					switch (currentState)
					{
					case State_GamePlay:
						handleMouseGamePlay(&game, x, y);
						break;
					case State_MainMenu:
						handleMouseMainMenu(x, y, &quit, &currentState);
						break;
					case State_ModeOptions:
						handleMouseModeOptions(x, y, &game, &currentState);
						break;
					case State_LoadGame:
						handleMouseLoadGame(x, y, &game, &currentState);
						break;
					default:
						break;
					}
				}
				else if (e.type == SDL_KEYDOWN)
				{
					if (currentState == State_GamePlay)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_LEFT:
							if (game.ai)
								undoMove(&game);

							undoMove(&game);
							renderPlayerText(&game);
							break;
						case SDLK_RIGHT:
							redoMove(&game);
							renderPlayerText(&game);
							break;
						case SDLK_ESCAPE:
							currentState = State_MainMenu;
							break;
						default:
							break;
						}
					}
					else if (currentState == State_Draw || currentState == State_ComputerWon)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_RETURN:
							currentState = State_MainMenu;
							break;
						default:
							break;
						}
					}
					else
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							currentState = State_MainMenu;
							break;
						default:
							break;
						}
					}
				}
				
				draw(&game, currentState, NULL);

				int winner = endGame(&game);
				if (winner && !game.gameEnded)
				{
					currentState = State_WinnerName;
					typing = true;
				}

				if (winner == 1)
					handleTyping(&e, &typing, &p1, &game, &currentState);
				else if (winner == 2)
					handleTyping(&e, &typing, &p2, &game, &currentState);
				else if (winner == 3)
				{
					currentState = State_Draw;
					typing = false;
					game = createNewGame();
				}
				
			}
		}
	}

	close(&game);
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
	for (int i = 1; i < BUTTONS_TotalMainMenu; ++i)
	{
		int x1 = buttons[i].rect.x, x2 = buttons[i].rect.x + buttons[i].rect.w;
		int y1 = buttons[i].rect.y, y2 = buttons[i].rect.y + buttons[i].rect.h;

		if (x1 <= x && x2 >= x && y1 <= y && y2 >= y)
		{
			switch (i)
			{
			case BUTTONS_newGame:
				*currentState = State_ModeOptions;
				break;
			case BUTTONS_resume:
				*currentState = State_GamePlay;
				break;
			case BUTTONS_loadGame:
				*currentState = State_LoadGame;
				break;
			case BUTTONS_highScore:
				*currentState = State_HighScore;
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

void handleMouseModeOptions(int x, int y, Game* game, enum State* currentState)
{
	for (int i = BUTTONS_HvH; i < BUTTONS_TotalMode; ++i)
	{
		int x1 = buttons[i].rect.x, x2 = buttons[i].rect.x + buttons[i].rect.w;
		int y1 = buttons[i].rect.y, y2 = buttons[i].rect.y + buttons[i].rect.h;

		if (x1 <= x && x2 >= x && y1 <= y && y2 >= y)
		{
			switch (i)
			{
			case BUTTONS_HvH:
				*currentState = State_GamePlay;
				*game = createNewGame();
				break;
			case BUTTONS_HvC:
				game->ai = true;
				*currentState = State_GamePlay;
				break;
			default:
				break;
			}
		}
	}
}

void handleMouseLoadGame(int x, int y, Game* game, enum State* currentState)
{
	for (int i = BUTTONS_game1; i < BUTTONS_TOTAL; ++i)
	{
		int x1 = buttons[i].rect.x, x2 = buttons[i].rect.x + buttons[i].rect.w;
		int y1 = buttons[i].rect.y, y2 = buttons[i].rect.y + buttons[i].rect.h;

		if (x1 <= x && x2 >= x && y1 <= y && y2 >= y)
		{
			*currentState = State_GamePlay;

			switch (i)
			{
			case BUTTONS_game1:
				loadGame(game, 1);
				renderPlayerText(game);
				break;
			case BUTTONS_game2:
				loadGame(game, 2);
				break;
			case BUTTONS_game3:
				loadGame(game, 3);
				break;
			default:
				break;
			}
		}
	}
}

void handleTyping(SDL_Event* e, bool* typing, Player* winner, Game* game, enum State* currentState)
{
	if (game->ai && winner->color == 'g')
	{
		*currentState = State_ComputerWon;
		*game = createNewGame();
		*typing = false;
		return;
	}

	SDL_StartTextInput();

	while (*typing)
	{
		while (SDL_WaitEvent(e))
		{
			endGameText(winner);
			draw(game, *currentState, winner);

			if (e->type == SDL_QUIT)
			{
				*typing = false;
				*currentState = State_MainMenu;
				break;
			}

			if (e->type == SDL_TEXTINPUT || e->type == SDL_KEYDOWN)
			{
				if (e->key.keysym.sym == SDLK_RETURN)
				{
					*typing = false;
					*currentState = State_MainMenu;
					break;
				}
				
				if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE && strlen(winner->name) > 1)
					winner->name[strlen(winner->name) - 1] = 0;

				else if (e->type == SDL_TEXTINPUT)
					strcat(winner->name, e->text.text);
			}
		}
	}

	SDL_StopTextInput();
	saveHighScore(winner->name, winner->score);
	highScoreText();
	game->gameEnded = true;
}