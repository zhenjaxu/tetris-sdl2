#pragma once
#include "SpriteComponent.h"
#include <memory>
#include <vector>

class BlockSpriteComponent : SpriteComponent
{
public:
    BlockSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Draw(class SDL_Renderer* renderer) override;

private:
    void DrawBlocks(class SDL_Renderer* renderer, const std::shared_ptr<std::vector<struct Block>>& blocks);
};