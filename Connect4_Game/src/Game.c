#include "include/Game.h"


void runGame()
{
	Game game = createNewGame();

	if (!init())
		printf("Failed to initialize!\n");
	else
	{
		loadMedia();
		
		bool quit = false;
		SDL_Event e;

		int x, y;
		bool r = true;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					quit = true;

				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					SDL_GetMouseState(&x, &y);
						
					int row = y / 100, col = x / 100;

					printf("row: %d  col: %d\n", row, col);

					makeMove(&game, col);
				}
			}

			draw(&game);
		}
	}

	close();
}
