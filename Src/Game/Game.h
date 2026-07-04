#pragma once
#include<SDL2/SDL.h>
#include<vector>

// 游戏主类：负责 SDL 初始化、主循环、资源管理与场景对象调度
class Game{
public:
    Game();
    bool Initialize();   // 初始化 SDL 窗口、渲染器与游戏数据
    void RunLoop();      // 游戏主循环：输入 -> 更新 -> 渲染
    void Shutdown();     // 释放资源并退出 SDL

    void AddActor(class Actor* actor);      // Actor 在构造时自动注册
    void RemoveActor(class Actor* actor);   // Actor 在析构时自动注销

    class Board* GetBoard() const {return mBoard;}
    int GetBoardColumns() const {return mBoardColumns;}
    int GetBoardCell() const {return mBoardCell;}

private:
    void ProcessInput();   // 处理窗口事件与键盘输入
    void UpdateGame();     // 基于 deltaTime 更新游戏逻辑
    void GenerateOutput(); // 渲染当前帧

    void LoadData();    // 创建 Board、Piece 等游戏对象
    void UnloadData();  // 统一释放所有 Actor 资源

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;

    const int mBoardColumns;
    const int mBoardRows;
    const int mBoardCell;

    std::vector<class Actor*> mActors;

    class Board* mBoard;
    class Piece* mPiece;
};