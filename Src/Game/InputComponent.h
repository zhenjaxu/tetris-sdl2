#pragma once
#include <cstdint>

class InputComponent
{
public:
    InputComponent(class Actor* owner);
    void ProcessInput(const uint8_t* keyState);

private:
    bool mPrevA;
    bool mPrevD;
    bool mPrevW;
    bool mPrevSpace;

    class Actor* mOwner;
};