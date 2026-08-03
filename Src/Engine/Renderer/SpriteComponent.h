#pragma once
#include "Component.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexWidth() const { return mTexWidth; }
    int GetTexHeight() const { return mTexHeight; }

private:
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;

    SDL_Texture* mTexture;
};