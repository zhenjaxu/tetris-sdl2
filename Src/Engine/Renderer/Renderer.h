#pragma once
#include <vector>
#include <string>

class Renderer
{
public:
    Renderer(class Game* game);
    bool Initialize();
    void Shutdown();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

private:
    void DrawText(const std::string& text, int x, int y, struct SDL_Color color);

    class SDL_Window* mWindow;
    class SDL_Renderer* mRenderer;
    std::vector<class SpriteComponent*> mSprites;

    class Game* mGame;

    class TTF_Font* font;
};