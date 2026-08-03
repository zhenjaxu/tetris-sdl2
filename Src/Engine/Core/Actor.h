#pragma once
#include "Math.h"
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

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    class Game* GetGame() const {return mGame;}

    State GetState() const { return mState; }
    const Vector2& GetPosition() const { return mPosition; }
    float GetScale() const { return mScale; }
    float GetRotation() const { return mRotation; }

    void SetState(State state){ mState = state; }
    void SetPosition(const Vector2& position){ mPosition = position; }
    void SetScale(float scale){ mScale = scale; }
    void SetRotation(float rotation){ mRotation = rotation; }

protected:
    State mState;
    Vector2 mPosition;
    float mScale;
    float mRotation;

    std::vector<class Component*> mComponents;
    class Game* mGame;
};