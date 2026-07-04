#include"Actor.h"
#include"Game.h"

Actor::Actor(Game* game)
:mGame(game)
{
    // 构造时自动注册到 Game 的 Actor 列表，实现生命周期自动管理
    mGame->AddActor(this);
}

Actor::~Actor(){
    // 析构时自动从 Game 的 Actor 列表移除
    mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime){}

void Actor::ProcessInput(const uint8_t* keyState){}

void Actor::Draw(SDL_Renderer* renderer){}