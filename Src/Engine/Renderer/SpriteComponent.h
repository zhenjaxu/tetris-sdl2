#pragma once
#include "Component.h"
#include <memory>
#include <vector>

class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    void Draw(class SDL_Renderer* renderer);

    int GetDrawOrder() const { return mDrawOrder; }
    
private:
    int mDrawOrder;
};