#pragma once
#include "Component.h"
#include <cstdint>

class InputComponent : Component
{
public:
    InputComponent(class Actor* owner);
    void ProcessInput(const uint8_t* keyState) override;

private:
    bool mPrevA;
    bool mPrevD;
    bool mPrevW;
    bool mPrevR;
    bool mPrevSpace;
};