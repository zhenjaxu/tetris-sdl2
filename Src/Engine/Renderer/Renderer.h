#pragma once
#include <vector>

class Renderer
{
public:
    Renderer(class Game* game);
    bool Initialize();
    void Shutdown();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    class TTF_Font* GetFont() const { return mFont; }

private:
    class SDL_Window* mWindow;
    class SDL_Renderer* mRenderer;
    std::vector<class SpriteComponent*> mSprites;

    class Game* mGame;

    class TTF_Font* mFont;
};