#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Config.h"
#include <SDL2/SDL.h>

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
: Component(owner)
, mDrawOrder(drawOrder)
{
    owner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    std::shared_ptr<std::vector<Block>> blocks = mOwner->DrawCall();

    for(auto block: *blocks)
    {
        if(block.blend) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        else SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_SetRenderDrawColor(renderer, block.color.r, block.color.g, block.color.b, block.color.a);

        SDL_Rect rc = {
            static_cast<int>(block.x),
            static_cast<int>(block.y),
            static_cast<int>(block.w),
            static_cast<int>(block.h)
        };

        SDL_RenderFillRect(renderer, &rc);
    }
}