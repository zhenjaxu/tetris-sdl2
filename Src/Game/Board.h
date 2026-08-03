#pragma once
#include "Actor.h"
#include "Config.h"
#include <vector>
#include <memory>

class Board : public Actor{
public:
    Board(class Game* game);

    bool IsValid(const std::vector<Vector2>& blocks) const;        // 碰撞检测：是否越界或与已固定方块重叠
    void Lock(const std::vector<Vector2>& blocks, int type);       // 将当前方块锁定到棋盘上
    void ClearLines();                  // 消除已满的行
    void Reset();                       // 清空棋盘

    std::shared_ptr<std::vector<Block>> GetBlocks();

private:
    std::vector<std::vector<int>> mGrid;
    class UI* mScoreCounter;
};