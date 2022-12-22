#include "include/Game.h"


void runGame()
{
	Game game = createNewGame();
	enum State currentState = State_MainMenu;
	 loadGame(&game,3);

	if (!init())
		printf("Failed to initialize!\n");
	else
	{
		loadMedia(&game);

		bool quit = false;
		SDL_Event e;

		int x, y;
		bool r = true;
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

					if (currentState == State_GamePlay)
						handleMouseGamePlay(&game, x, y);
					else if (currentState == State_MainMenu)
						handleMouseMainMenu(x, y, &quit, &currentState);
				}
				else if (e.type == SDL_KEYDOWN)
				{
					if (currentState == State_GamePlay)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_LEFT:
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
						case SDLK_s:
							saveGame(&game,1);saveGame(&game,2);updateLast(game);
							break;
						default:
							break;
						}
					}
				}

				draw(&game, currentState);
			}
		}
	}

	close(&game);
}
