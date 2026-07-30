#pragma once
#include "Actor.h"
#include "Config.h"
#include <vector>

class Board : public Actor{
public:
    Board(class Game* game);

    void Draw(class SDL_Renderer* renderer) override;

    bool IsValid(const std::vector<Vector2>& blocks) const;        // 碰撞检测：是否越界或与已固定方块重叠
    void Lock(const std::vector<Vector2>& blocks, int type);       // 将当前方块锁定到棋盘上
    void ClearLines();                  // 消除已满的行
    void Reset();                       // 清空棋盘

private:
    std::vector<std::vector<int>> mGrid;
};