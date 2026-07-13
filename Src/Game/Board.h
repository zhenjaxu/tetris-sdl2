#pragma once
#include"Actor.h"
#include<SDL2/SDL.h>
#include<vector>

// Board 类：表示游戏背景与已固定的方块
// 通过 mGrid 记录每个格子的状态，-1 表示空，其他数字对应方块颜色
class Board:public Actor{
public:
    Board(class Game* game, int columns, int rows, int cell);

    void Draw(SDL_Renderer* renderer) override;

    bool IsValid(const Vector2 blocks[4]) const;        // 碰撞检测：是否越界或与已固定方块重叠
    void Lock(const Vector2 blocks[4], int type);       // 将当前方块锁定到棋盘上
    void ClearLines();                                      // 消除已满的行
    void Reset();                                           // 清空棋盘

    int GetColumns() const {return mColumns;}
    int GetRows() const {return mRows;}
    int GetCell() const {return mCell;}

    const SDL_Color* GetColors() const {return mColors;}

private:
    std::vector<std::vector<int>> mGrid;
    int mColumns;
    int mRows;
    int mCell;

    const SDL_Color mColors[7];
};