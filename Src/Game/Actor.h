#pragma once
#include<cstdint>
#include<SDL2/SDL.h>

// 二维整数向量，用于表示方块格子坐标
struct Vector2{int x, y;};

// Actor 基类：所有游戏对象的抽象
// 提供 Update/ProcessInput/Draw 虚函数，由 Game 统一调度
class Actor{
public:
    Actor(class Game* game);
    virtual ~Actor();

    virtual void Update(float deltaTime);
    virtual void ProcessInput(const uint8_t* keyState);
    virtual void Draw(SDL_Renderer* renderer);

    class Game* GetGame() const {return mGame;}

private:
    class Game* mGame;
};