#include "Game.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Config.h"
#include "Renderer.h"
#include <algorithm>

Game::Game()
: mRenderer(nullptr)
, mIsRunning(true)
, mPiece(nullptr)
, mTicksCount(0)
{}



bool Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(this);
    if(!mRenderer->Initialize())
    {
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }

    LoadData();
    mTicksCount = SDL_GetTicks();
    return true;
}

void Game::RunLoop()
{
    while(mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    UnloadData();
    if(mRenderer)
    {
        mRenderer->Shutdown();
    }
    SDL_Quit();
}



void Game::ProcessInput()
{
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

void Game::UpdateGame()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+Config::TICK_DT));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    if(deltaTime>0.05f) deltaTime=0.05f;
    mTicksCount=SDL_GetTicks();

    mPiece->Update(deltaTime);
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}



void Game::LoadData()
{
    auto board = new Board(this);
    mPiece = new Piece(this, board);
}

void Game::UnloadData()
{
    while(!mActors.empty())
    {
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