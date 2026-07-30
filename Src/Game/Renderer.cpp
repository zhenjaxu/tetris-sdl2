#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "Config.h"
#include <SDL2/SDL.h>

Renderer::Renderer(Game* game)
: mGame(game)
, mWindow(nullptr)
, mRenderer(nullptr)
{}

bool Renderer::Initialize()
{
    mWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, 0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Renderer::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
    SDL_SetRenderDrawColor(mRenderer, 10, 10, 10, 255);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 30, 30, 30, 255);
    SDL_Rect rc = {0, 0, (int)Config::BOARD_WIDTH, (int)Config::BOARD_HEIGHT};
    SDL_RenderFillRect(mRenderer, &rc);

    for(auto actor : mGame->GetActors())
    {
        actor->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}