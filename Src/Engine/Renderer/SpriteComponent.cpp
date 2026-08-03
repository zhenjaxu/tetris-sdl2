#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Math.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
: Component(owner)
, mDrawOrder(drawOrder)
, mTexWidth(0)
, mTexHeight(0)
{
    owner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
    mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    SDL_Rect r;
    r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

    SDL_RenderCopyEx(renderer, mTexture, nullptr, &r,
        -Math::ToDegrees(mOwner->GetRotation()), nullptr,
        SDL_FLIP_NONE
    );
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
    mTexture = texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}