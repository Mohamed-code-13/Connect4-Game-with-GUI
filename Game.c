#include "include/Game.h"


void runGame()
{
    Game game = createNewGame();

    if (!init())
        printf("Failed to initialize!\n");
    else
    {
        loadMedia();
        createButtons();
        bool quit = false;
        bool quit2 = false;
        SDL_Event e;
        int mWindowID= SDL_GetWindowID(mWindow);

        int x, y;
        bool r = true;
        while (!quit)
        {
            while (SDL_PollEvent(&e))
            {
                SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

                if (e.type == SDL_WINDOWEVENT
                        && e.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    quit = true;
                }
                else if (e.type ==SDL_MOUSEBUTTONDOWN&&e.window.windowID == mWindowID)
                {
                    SDL_GetMouseState(&x,&y);
                    if(50< x && x<450 && 10<y && y<90)
                    {
                        renderChooseMode();
                        while(!quit2)
                        {
                            SDL_PollEvent(&e);
                            {
                                if (e.type == SDL_WINDOWEVENT
                                        && e.window.event == SDL_WINDOWEVENT_CLOSE)
                                {
                                    quit=true;
                                    quit2= true;
                                }
                                if (e.type ==SDL_MOUSEBUTTONDOWN)
                                {
                                    SDL_GetMouseState(&x,&y);
                                    if(y<mainMenuHeight/2)
                                    {
                                        SDL_DestroyWindow(mWindow);
                                        SDL_ShowWindow(gWindow);
                                        quit2=true;
                                    }
                                }
                            }
                        }
                    }
                    else if(50< x && x<450 && 10+300<y && y<90+300)
                    {
                        quit = true;
                    }

                }

                else if (e.type == SDL_MOUSEBUTTONDOWN )
                {
                    SDL_GetMouseState(&x, &y);

                    {

                        int row = y / 100, col = x / 100;

                        printf("row: %d  col: %d\n", row, col);

                        makeMove(&game, col);
                    }
                }
            }

            draw(&game);
            renderMainMenu();
        }
    }

    close();
}
