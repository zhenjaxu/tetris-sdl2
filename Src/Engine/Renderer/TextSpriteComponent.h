#pragma once
#include "SpriteComponent.h"
#include <memory>
#include <vector>
#include "Config.h"
#include <SDL2/SDL.h>

class TextSpriteComponent : SpriteComponent
{
public:
    TextSpriteComponent(class Actor* owner, int drawOrder = 100);
    void Draw(class Shader* shader) override;

private:
    void DrawTexts(class Shader* shader, std::shared_ptr<std::vector<Text>> texts);
};