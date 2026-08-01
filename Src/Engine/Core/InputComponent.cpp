#include "InputComponent.h"
#include "Actor.h"
#include "Piece.h"
#include "Config.h"
#include <SDL2/SDL_scancode.h>

InputComponent::InputComponent(Actor* owner)
: Component(owner)
, mPrevA(false)
, mPrevD(false)
, mPrevW(false)
, mPrevSpace(false)
{}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
    bool A = keyState[SDL_SCANCODE_A];
    bool D = keyState[SDL_SCANCODE_D];
    bool W = keyState[SDL_SCANCODE_W];
    bool Space = keyState[SDL_SCANCODE_SPACE];

    if(auto find = dynamic_cast<Piece*>(mOwner))
    {
        if(A && !mPrevA) find->SendInput(Piece::LEFT);
        if(D && !mPrevD) find->SendInput(Piece::RIGHT);
        if(W && !mPrevW) find->SendInput(Piece::ROTATE);
        if(Space && !mPrevSpace) find->SendInput(Piece::HARD_DROP);

        find->SetDropSpeed(keyState[SDL_SCANCODE_S] ? Config::SOFT_DROP : Config::DROP_SPEED);
    }

    mPrevA = A;
    mPrevD = D;
    mPrevW = W;
    mPrevSpace = Space;
}