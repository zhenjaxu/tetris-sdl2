#include"Game.h"
#include"Actor.h"
#include"Board.h"
#include"Piece.h"
#include<algorithm>

// 初始化窗口、渲染器、Actor 容器与游戏配置参数
Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mBoard(nullptr)
,mPiece(nullptr)
,mBoardColumns(10)
,mBoardRows(20)
,mBoardCell(30.0f)
,mTicksCount(0)
{}

bool Game::Initialize(){
    // 初始化 SDL 视频子系统
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // 窗口尺寸由棋盘列数、行数与格子大小决定
    mWindow=SDL_CreateWindow("Tetris", 200, 100, mBoardColumns*mBoardCell, mBoardRows*mBoardCell, 0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // 创建硬件加速渲染器，开启垂直同步避免画面撕裂
    mRenderer=SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    LoadData();
    // 记录初始 tick，用于计算 deltaTime
    mTicksCount=SDL_GetTicks();
    return true;
}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){
    UnloadData();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::ProcessInput(){
    // 处理窗口关闭等事件
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mIsRunning=false;
                break;
        }
    }

    // 获取当前键盘状态，并转发给 Piece 处理移动/旋转/硬降
    const Uint8* keyState=SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE]){
        mIsRunning=false;
    }

    mPiece->ProcessInput(keyState);
}

void Game::UpdateGame(){
    // 固定约 60 FPS 的主循环节奏
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    // 防止断点/卡顿导致 deltaTime 过大
    if(deltaTime>0.05f) deltaTime=0.05f;
    mTicksCount=SDL_GetTicks();

    mPiece->Update(deltaTime);
}

void Game::GenerateOutput(){
    // 清屏为深色背景
    SDL_SetRenderDrawColor(mRenderer, 30, 30, 30, 255);
    SDL_RenderClear(mRenderer);

    // 先绘制已固定的 Board，再绘制当前活动 Piece
    mBoard->Draw(mRenderer);
    mPiece->Draw(mRenderer);

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData(){
    mBoard=new Board(this, mBoardColumns, mBoardRows, mBoardCell);

    mPiece=new Piece(this, 0.5f, 0.1f);
}

void Game::UnloadData(){
    // 通过 delete Actor 触发析构函数自动从列表移除
    while(!mActors.empty()){
        delete mActors.back();
    }
}

void Game::AddActor(Actor* actor){
    mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor){
    // O(1) 移除：与末尾交换后 pop_back
    auto iter=std::find(mActors.begin(), mActors.end(), actor);
    if(iter!=mActors.end()){
        std::iter_swap(iter, mActors.end()-1);
        mActors.pop_back();
    }
}