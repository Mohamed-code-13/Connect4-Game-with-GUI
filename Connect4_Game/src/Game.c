#include "include/Game.h"


void runGame()
{
	Game game = createNewGame();
	loadGame(&game);
	
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
				if (e.type == SDL_QUIT)
				{
					quit = true;
					break;
				}

				else if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					SDL_GetMouseState(&x, &y);
						
					int row = (y - 80) / 80, col = x / 80;

					printf("row: %d  col: %d\n", row, col);

					makeMove(&game, col);

					renderText(&game);

					printf("Score player1: %d\n", p1.score);
					printf("Score player2: %d\n", p2.score);
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT:
						undoMove(&game);
						renderText(&game);
						break;
					case SDLK_RIGHT:
						redoMove(&game);
						renderText(&game);
						break;
					default:
						break;
					}
				}

				draw(&game);
			}
		}
	}

	close(&game);
}
