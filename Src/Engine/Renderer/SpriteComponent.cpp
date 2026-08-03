#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Math.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
: Component(owner)
, mTexture(nullptr)
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

void SpriteComponent::Draw(Shader* shader)
{
    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(mTexWidth),
        static_cast<float>(mTexHeight),
        1.0f
    );

    Matrix4 world = scaleMat * mOwner->GetWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", world);

    mTexture->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // SDL_Rect r;
    // r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
    // r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
    // r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
    // r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

    // SDL_RenderCopyEx(renderer, mTexture, nullptr, &r,
    //     -Math::ToDegrees(mOwner->GetRotation()), nullptr,
    //     SDL_FLIP_NONE
    // );
}

void SpriteComponent::SetTexture(Texture* texture)
{
    mTexture = texture;
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();

    // mTexture = texture;
    // SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}