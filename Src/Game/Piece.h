#pragma once
#include"Actor.h"

// Piece 类：表示玩家当前控制的俄罗斯方块
// 负责处理输入、自动下落、旋转、绘制以及幽灵块/硬降逻辑
class Piece:public Actor{
public:
    Piece(class Game* game, float speed, float dropSpeed);

    void Update(float deltaTime) override;
    void ProcessInput(const uint8_t* keyState) override;
    void Draw(SDL_Renderer* renderer) override;

private:
    void Spawn();                                // 随机生成新方块
    void Rotate(Vector2 out[4]) const;           // 顺时针旋转，结果写入 out
    void CalculateGhost(Vector2 ghost[4]) const; // 计算方块落到底部的投影

    Vector2 mBlocks[4];
    Vector2 mGhost[4];
    Vector2 mPosition;
    int mType;

    bool mPrevA;
    bool mPrevD;
    bool mPrevW;
    bool mPrevSpace;
    
    float mSpeed;
    float mDropSpeed;
    float mDropAccumulate;
};