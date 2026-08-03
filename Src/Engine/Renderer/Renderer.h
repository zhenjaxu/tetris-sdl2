#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Renderer
{
public:
    Renderer(class Game* game);
    bool Initialize();
    void UnloadData();
    void Shutdown();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    TTF_Font* GetFont() const { return mFont; }
    class Texture* GetTexture(const std::string& fileName);

private:
    bool LoadShaders();
    void CreateSpriteVerts();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_GLContext mContext;
    std::vector<class SpriteComponent*> mSprites;

    class Game* mGame;

    TTF_Font* mFont;

    class VertexArray* mSpriteVerts;
    class Shader* mSpriteShader;

    std::unordered_map<std::string, class Texture*> mTextures;
};