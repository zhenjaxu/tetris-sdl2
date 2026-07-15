#include"InputManager.h"

void InputManager::PushEvent(SDL_Event& event){
    switch(event.key.keysym.scancode){
        case SDL_SCANCODE_W:
            mEvents.push_back(ROTATE);
            break;
        case SDL_SCANCODE_SPACE:
            mEvents.push_back(HARD_DROP);
            break;
    }
}

void InputManager::ProcessInput(const uint8_t* keyState){
    memcpy(mPrev, mCurr, SDL_NUM_SCANCODE);
    memcpy(mCurr, keyState, SDL_NUM_SCANCODE);
}

void InputManager::EndFrame(){
    mEvents.clear();
}