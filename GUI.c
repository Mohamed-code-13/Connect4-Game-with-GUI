#include "include/GUI.h"

const char* pathsCirclesImages[] = { "./images/wC.png", "./images/rC.png", "./images/gC.png" };


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

        gWindow = SDL_CreateWindow("Connect 4 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_HIDDEN);
        int gWindowID= SDL_GetWindowID(gWindow);

        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        mWindow = SDL_CreateWindow("MainMenu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mainMenuWidth, mainMenuHeight, SDL_WINDOW_SHOWN);

        if (mWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL || mRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
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

void loadMedia()
{
    for (int i = 0; i < Colors_TOTAL; ++i)
    {
        loadFromFile(&circles[i], pathsCirclesImages[i]);
    }
    gFont= TTF_OpenFont("acme.ttf",60);
    if(gFont==NULL)printf("Failed to load font!");



}

SDL_Color color= {232,234,11,255};
void writeButton(Button *B)
{

    msurface = TTF_RenderText_Solid(gFont,B->label,color);//if(gFont==NULL){printf("lala");}
    B->mTexture = SDL_CreateTextureFromSurface(mRenderer,msurface);
    SDL_FreeSurface(msurface);

}

void createButtons()
{


    StartButton.label=    "Start a new game ";
    LoadButton.label=     "   Load a game   ";
    TopScoreButton.label= "    Top players  ";
    QuitButton.label=     "      Quit       ";
    vsHumanButton.label=  "    VS HUMAN     ";
    vsCompButton.label=   "   VS COMPUTER   ";

    msurface = TTF_RenderText_Solid(gFont,StartButton.label,color);
    stexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);

    msurface = TTF_RenderText_Solid(gFont,LoadButton.label,color);
    ltexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);

    msurface = TTF_RenderText_Solid(gFont,TopScoreButton.label,color);
    ttexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);

    msurface = TTF_RenderText_Solid(gFont,QuitButton.label,color);
    qtexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);

    msurface = TTF_RenderText_Solid(gFont,vsHumanButton.label,color);
    htexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);

    msurface = TTF_RenderText_Solid(gFont,vsCompButton.label,color);
    ctexture = SDL_CreateTextureFromSurface(mRenderer,msurface);SDL_FreeSurface(msurface);
}

void draw(Game* game)
{
    SDL_SetRenderDrawColor(gRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

    for (int i = 10; i < SCREEN_HEIGHT; i += CIRCLE_SIZE + 20)
    {
        for (int j = 10; j < SCREEN_WIDTH; j += CIRCLE_SIZE + 20)
        {
            enum Colors current;

            switch (game->board[i / 100][j / 100])
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

            SDL_RenderDrawLine(gRenderer, j - 10, 0, j - 10, SCREEN_HEIGHT);
        }

        SDL_RenderDrawLine(gRenderer, 0, i - 10, SCREEN_WIDTH, i - 10);
    }

    SDL_RenderPresent(gRenderer);

}

void renderMainMenu()
{

    SDL_SetRenderDrawColor(mRenderer, 0x3E, 0x3E, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);

    SDL_Rect rectStartNewGame = { 50, 10, 400, 80 };
    SDL_RenderFillRect(mRenderer,&rectStartNewGame);

    SDL_Rect rectLoadGame = {50, 110, 400, 80 };
    SDL_RenderFillRect(mRenderer,&rectLoadGame);

    SDL_Rect rectTopScore = { 50, 210, 400, 80 };
    SDL_RenderFillRect(mRenderer,&rectTopScore);

    SDL_Rect rectQuit = { 50, 310, 400, 80 };
    SDL_RenderFillRect(mRenderer,&rectQuit);



    SDL_RenderCopy(mRenderer,stexture,NULL, &rectStartNewGame);

    SDL_RenderCopy(mRenderer,ltexture,NULL, &rectLoadGame);

    SDL_RenderCopy(mRenderer,ttexture,NULL, &rectTopScore);

    SDL_RenderCopy(mRenderer,qtexture,NULL, &rectQuit);

    SDL_RenderPresent(mRenderer);
}

void renderChooseMode()
{
    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect rectHuman = { 50, 50, 400, 100 };
    SDL_Rect rectComp = { 50, 200, 400, 100 };
    SDL_RenderFillRect(mRenderer,&rectHuman);
    SDL_RenderFillRect(mRenderer,&rectComp);
    SDL_RenderCopy(mRenderer,htexture,NULL,&rectHuman);
    SDL_RenderCopy(mRenderer,ctexture,NULL,&rectComp);
    SDL_RenderPresent(mRenderer);

}

void close()
{
    for (int i = 0; i < Colors_TOTAL; ++i)
    {
        freeTexture(&circles[i]);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_DestroyWindow(mWindow);
    gRenderer = NULL;
    mRenderer = NULL;
    gWindow = NULL;
    mWindow = NULL;
    TTF_CloseFont(gFont);
    gFont = NULL;
    SDL_DestroyTexture(mtexture);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

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
            texture->mHeight = loadSurface->h;
            texture->mWidth = loadSurface->w;
        }

        SDL_FreeSurface(loadSurface);
    }

    texture->mTexture = newTexture;
    return texture->mTexture != NULL;
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

void render(Texture* tex, int x, int y)
{
    SDL_Rect rect = { x, y, tex->mWidth, tex->mHeight };
    SDL_RenderCopy(gRenderer, tex->mTexture, NULL, &rect);

}
void renderButton(Texture* tex, int x, int y)
{
    SDL_Rect rect = { x, y, tex->mWidth, tex->mHeight };

    SDL_RenderCopy(mRenderer, tex->mTexture, NULL, &rect);

}

