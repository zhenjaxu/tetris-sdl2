#pragma once
#include "SpriteComponent.h"
#include <memory>
#include <vector>

class TextSpriteComponent : SpriteComponent
{
public:
    TextSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Draw(class SDL_Renderer* renderer) override;

private:
    void DrawTexts(class SDL_Renderer* renderer, std::shared_ptr<std::vector<struct Text>> texts);
};