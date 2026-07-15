#include"Actor.h"
#include"Game.h"

Actor::Actor(Game* game)
:mGame(game)
{
    // 构造时自动注册到 Game 的 Actor 列表，实现生命周期自动管理
    mGame->AddActor(this);
}

Actor::~Actor(){
    while(!mComponent.empty()){
        delete mComponent.back();
    }

    // 析构时自动从 Game 的 Actor 列表移除
    mGame->RemoveActor(this);
}

void Actor::Update(float updateTime){
    if(mState==EActive){
        UpdateComponents(updateTime);
        UpdateActor(updateTime);
    }
}

void Actor::UpdateComponents(float updateTime){
    for(auto comp:mComponents){
        comp->Update(updateTime);
    }
}

void Actor::UpdateActor(float updateTime){}

void Actor::Draw(SDL_Renderer* renderer){}

void Actor::AddComponent(Component* component){
    int myOrder=component->GetUpdateOrder();
    auto iter=mComponents.begin();

    for(;iter!=mComponents.end();++iter){
        if(myOrder<(*iter)->GetUpdateOrder()){
            break;
        }
    }

    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component){
    auto iter=std::find(mComponents.begin(), mComponents.end(), component);
    if(iter!=mComponents.end()) mComponents.erase(iter);
}