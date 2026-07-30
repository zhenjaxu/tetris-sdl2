#include "Game.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Config.h"
#include <algorithm>

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mBoard(nullptr)
,mPiece(nullptr)
,mTicksCount(0)
{}



bool Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow=SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, 0);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer=SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer){
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    LoadData();
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
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mIsRunning=false;
                break;
        }
    }

    const Uint8* keyState=SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE]){
        mIsRunning=false;
    }

    mPiece->ProcessInput(keyState);
}

void Game::UpdateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+Config::TICK_DT));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    if(deltaTime>0.05f) deltaTime=0.05f;
    mTicksCount=SDL_GetTicks();

    mPiece->Update(deltaTime);
}

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(mRenderer, 10, 10, 10, 255);
    SDL_RenderClear(mRenderer);
    SDL_SetRenderDrawColor(mRenderer, 30, 30, 30, 255);
    SDL_Rect rc = {0, 0, (int)Config::BOARD_WIDTH, (int)Config::BOARD_HEIGHT};
    SDL_RenderFillRect(mRenderer, &rc);

    mBoard->Draw(mRenderer);
    mPiece->Draw(mRenderer);

    SDL_RenderPresent(mRenderer);
}



void Game::LoadData(){
    mBoard=new Board(this);
    mPiece=new Piece(this);
}

void Game::UnloadData(){
    while(!mActors.empty()){
        delete mActors.back();
    }
}



void Game::AddActor(Actor* actor){
    mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor){
    auto iter=std::find(mActors.begin(), mActors.end(), actor);
    if(iter!=mActors.end()){
        std::iter_swap(iter, mActors.end()-1);
        mActors.pop_back();
    }
}