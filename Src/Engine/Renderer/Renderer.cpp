#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "Config.h"
#include "SpriteComponent.h"
#include "Board.h"
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }

    font = TTF_OpenFont("Assets/ARIAL.TTF", 18);
    if (!font) {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
        return false;
    }

    return true;
}

void Renderer::Shutdown()
{
    if (font) TTF_CloseFont(font);
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

    SDL_Color c = {255, 255, 255, 255};

    DrawText("Last piece: ", int(Config::BOARD_WIDTH + 10), int(Config::BOARD_CELL * 1), c);
    DrawText("Last scores: ", int(Config::BOARD_WIDTH + 10), int(Config::BOARD_CELL * 6), c);
    DrawText(std::to_string(mGame->GetBoard()->GetLastScore()), int(Config::BOARD_WIDTH + 20), int(Config::BOARD_CELL * 7), c);
    DrawText("Current scores: ", int(Config::BOARD_WIDTH + 10), int(Config::BOARD_CELL * 8), c);
    DrawText(std::to_string(mGame->GetBoard()->GetScore()), int(Config::BOARD_WIDTH + 20), int(Config::BOARD_CELL * 9), c);

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



void Renderer::DrawText(const std::string& text, int x, int y, SDL_Color color) 
{
    if (text.empty()) return;

    // 1. 渲染成 surface（这里用 blended 质量最好，也可用 TTF_RenderText_Solid 更快）
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return;

    // 2. 转成 texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    
    // 3. 设置目标位置并渲染
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(mRenderer, texture, nullptr, &dstRect);

    // 4. 清理
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}