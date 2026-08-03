#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "Config.h"
#include "SpriteComponent.h"
#include <algorithm>

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

    // 字体初始化
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("Assets/ARIAL.TTF", 18);
    if (!mFont) {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
        return false;
    }

    return true;
}

void Renderer::Shutdown()
{
    if (mFont) TTF_CloseFont(mFont);
    TTF_Quit();

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

    for(auto sprite : mSprites)
    {
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if(iter != mSprites.end()) mSprites.erase(iter);
}