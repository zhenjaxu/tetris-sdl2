#pragma once
#include <cstdint>
#include <vector>

class Actor{
public:
    enum State
    {
        EActive,
        EDead,
        EPaused
    };

    Actor(class Game* game);
    virtual ~Actor();

    void Update(float deltaTime);
    virtual void UpdateActor(float deltaTime){}
    void ProcessInput(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState){}
    virtual void Draw(class SDL_Renderer* renderer){}

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    class Game* GetGame() const {return mGame;}

    State GetState() const { return mState; }
    void SetState(State state){ mState = state; }

private:
    State mState;
    std::vector<class Component*> mComponents;

    class Game* mGame;
};