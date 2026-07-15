#pragma once
#include<SDL2/SDL.h>
#include<vector>
#include<cstdint>

class InputManager{
public:
    enum GameEvent{
        ROTATE, 
        HARD_DROP
    };

    void PushEvent(SDL_Event& event);
    void ProcessInput(const uint8_t* keyState);
    void EndFrame();

    const std::vector<GameEvent>& GetEvents() const {return mEvents;}
    bool IsHeld(SDL_Scancode key) const {return mCurr[key]&&mPrev[key];}
 
private:
    std::vector<GameEvent> mEvents;

    Uint8 mCurr[SDL_NUM_SCANCODE];
    Uint8 mPrev[SDL_NUM_SCANCODE];
};