#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Renderer
{
public:
    Renderer(class Game* game);
    bool Initialize();
    void Shutdown();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    TTF_Font* GetFont() const { return mFont; }

private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    std::vector<class SpriteComponent*> mSprites;

    class Game* mGame;

    TTF_Font* mFont;
};