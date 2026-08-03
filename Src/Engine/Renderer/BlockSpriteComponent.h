#pragma once
#include "SpriteComponent.h"
#include "Config.h"
#include <memory>
#include <vector>

class BlockSpriteComponent : SpriteComponent
{
public:
    BlockSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Draw(class Shader* shader) override;

private:
    void DrawBlocks(class Shader* shader, const std::shared_ptr<std::vector<Block>>& blocks);
};