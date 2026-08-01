#pragma once
#include "Component.h"

class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class SDL_Renderer* renderer);
    virtual void SetTexture(class SDL_Texture* texture);

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexWidth() const { return mTexWidth; }
    int GetTexHeight() const { return mTexHeight; }

private:
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;

    class SDL_Texture* mTexture;
};