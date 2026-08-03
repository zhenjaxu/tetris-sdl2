#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(EActive)
, mPosition(Vector2{0, 0})
, mScale(1.0f)
, mRotation(0)
{
    mGame->AddActor(this);
}

Actor::~Actor()
{
    while(!mComponents.empty())
    {
        delete mComponents.back();
    }

    mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime)
{
    if(mState == EActive)
    {
        for(auto comp : mComponents)
        {
            comp->Update(deltaTime);
        }
        UpdateActor(deltaTime);
    }
}

void Actor::ProcessInput(const uint8_t* keyState)
{
    if(mState == EActive)
    {
        for(auto comp : mComponents)
        {
            comp->ProcessInput(keyState);
        }
        ActorInput(keyState);
    }
}

void Actor::AddComponent(Component* component)
{
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); ++iter)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}