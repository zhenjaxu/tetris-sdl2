#pragma once

class Renderer
{
public:
    Renderer(class Game* game);
    bool Initialize();
    void Shutdown();
    void Draw();

private:
    class SDL_Window* mWindow;
    class SDL_Renderer* mRenderer;
    class Game* mGame;
};