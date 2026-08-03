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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->SetBoolUniform("uUseTexture", true);

    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(mTexWidth),
        static_cast<float>(mTexHeight),
        1.0f
    );

    Matrix4 world = scaleMat * mOwner->GetWorldTransform();
    shader->SetMatrixUniform("uWorldTransform", world);

    mTexture->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SpriteComponent::SetTexture(Texture* texture)
{
    mTexture = texture;
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}