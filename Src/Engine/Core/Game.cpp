#include "Game.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Config.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include <algorithm>
#include <SDL2/SDL.h>

Game::Game()
: mRenderer(nullptr)
, mIsRunning(true)
, mPiece(nullptr)
, mTicksCount(0)
{}



bool Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
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

    mAudioSystem = new AudioSystem(this);
    if(!mAudioSystem->Initialize())
    {
        delete mAudioSystem;
        mAudioSystem = nullptr;
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
    if(mRenderer) mRenderer->Shutdown();
    if(mAudioSystem) mAudioSystem->Shutdown();
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

    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
    if(keyState[SDL_SCANCODE_R] && !reset)
    {
        mBoard->Reset();
        mPiece->Spawn();
    }
    reset = keyState[SDL_SCANCODE_R];

    mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
    if(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + Config::TICK_DT))
    {
        SDL_Delay(mTicksCount + Config::TICK_DT - SDL_GetTicks());
    }

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if(deltaTime > 0.05f) deltaTime = 0.05f;
    mTicksCount = SDL_GetTicks();

    mUpdatingActors=true;
    for(auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors=false;

    for(auto pending:mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for(auto actor:mActors)
    {
        if(actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    for(auto actor:deadActors)
    {
        delete actor;
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
    mAudioSystem->PlayBGM();
}



void Game::LoadData()
{
    mBoard = new Board(this);
    mPiece = new Piece(this, mBoard);
}

void Game::UnloadData()
{
    while(!mActors.empty())
    {
        delete mActors.back();
    }

    if(mAudioSystem) mAudioSystem->UnloadData();
}



void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}