#pragma once
#include<cstdint>
#include<SDL2/SDL.h>
#include"Math.h"

// Actor 基类：所有游戏对象的抽象
class Actor{
public:
    enum State{
        EActive,
        EDead,
        EPaused
    };

    Actor(class Game* game);
    virtual ~Actor();

    void Update(float updateTime);
    void UpdateComponents(float updateTime);
    virtual void UpdateActor(float updateTime);

    virtual void Draw(SDL_Renderer* renderer);

    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    template<typename T>
    T* GetComponent(){
        for(auto comp:mComponents){
            if(auto cast=dynamic_cast<T*>(comp)){
                return cast;
            }
        }
        return nullptr;
    }

    class Game* GetGame() const {return mGame;}
    State GetState() const {return mState;}
    Vector2 GetPosition() const {return mPosition;}
    void SetPosition(Vector2 pos){mPosition=pos;}

private:
    State mState;
    Vector2 mPosition;
    class Game* mGame;
    std::vector<class Component*> mComponents;
};