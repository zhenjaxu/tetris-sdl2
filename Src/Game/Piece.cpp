#include "Piece.h"
#include "Board.h"
#include "Game.h"
#include "InputComponent.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

Piece::Piece(Game* game, Board* board)
: Actor(game)
, mBoard(board)
, mBlocks(4)
, mGhost(4)
, mPosition(Vector2{0,0})
, mType(0)
, mDropTime(Config::DROP_TIME)
, mDropAccum(0.0f)
{
    auto im = new InputComponent(this);

    srand((unsigned)time(nullptr));
    Spawn();
}



void Piece::UpdateActor(float deltaTime)
{
    mDropAccum += deltaTime;

    for(auto input : mInputEvent)
    {
        Move(input);
        CalculateGhost(mGhost);
    }
    mInputEvent.clear();

    if(mDropAccum >= mDropTime)
    {
        mDropAccum -= mDropTime;
        Move(DROP);
    }
    
}

void Piece::Draw(SDL_Renderer* renderer)
{
    Color c = Config::COLORS[mType];
    SDL_Rect rc;
    rc.w = Config::BOARD_CELL - 2;
    rc.h = Config::BOARD_CELL - 2;

    for(int i = 0; i < 4; ++i)
    {
        if(mGhost[i].y<0) continue;   

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // 混合模式，绘制时与底色混合
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 80);
        rc.x = mGhost[i].x * Config::BOARD_CELL + 1;
        rc.y = mGhost[i].y * Config::BOARD_CELL + 1;
        SDL_RenderFillRect(renderer, &rc);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);   // 恢复正常模式，绘制实体块

        if(mBlocks[i].y<0) continue;

        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        rc.x = mBlocks[i].x * Config::BOARD_CELL + 1;
        rc.y = mBlocks[i].y * Config::BOARD_CELL + 1;
        SDL_RenderFillRect(renderer, &rc);
    }
}



void Piece::SendInput(MoveType move)
{
    mInputEvent.emplace_back(move);
}



void Piece::Lock()
{
    mBoard->Lock(mBlocks, mType);
    mBoard->ClearLines();
    Spawn();

    if(!mBoard->IsValid(mBlocks)) 
    {
        mBoard->Reset();
        Spawn();
    }
}

void Piece::Spawn(){
    mType = rand() % 7;
    mPosition.x = Config::BOARD_COLUMN / 2;
    mPosition.y = 1;
    for(int i = 0; i < 4; ++i)
    {
        mBlocks[i].x = Config::SHAPES[mType][i].x + mPosition.x;
        mBlocks[i].y = Config::SHAPES[mType][i].y + mPosition.y;
    }
    
    CalculateGhost(mGhost);
}

bool Piece::IsValid(const std::vector<Vector2>& blocks)
{
    if(mBoard->IsValid(blocks))
    {
        for(int i = 0; i < 4; ++i) mBlocks[i] = blocks[i];
        return true;
    }

    return false;
}

void Piece::CalculateGhost(std::vector<Vector2>& ghost) const
{
    for(int i = 0; i < 4; ++i) ghost[i] = mBlocks[i];

    while(true)
    {
        std::vector<Vector2> nxt(4);
        for(int i = 0; i < 4; ++i) nxt[i]= {ghost[i].x, ghost[i].y + 1};
        if(!mBoard->IsValid(nxt)) break;
        for(int i = 0; i < 4; ++i) ghost[i] = nxt[i];
    }
}



void Piece::Move(MoveType move)
{
    std::vector<Vector2> nxt(4);

    switch(move)
    {
        case LEFT:
            for(int i = 0; i < 4; ++i) nxt[i] = {mBlocks[i].x - 1, mBlocks[i].y};
            if(IsValid(nxt)) --mPosition.x;
            break;

        case RIGHT:
            for(int i = 0; i < 4; ++i) nxt[i] = {mBlocks[i].x + 1, mBlocks[i].y};
            if(IsValid(nxt)) ++mPosition.x;
            break;

        case DROP:
            for(int i = 0; i < 4; ++i) nxt[i] = {mBlocks[i].x, mBlocks[i].y + 1};
            if(IsValid(nxt)) ++mPosition.y;
            else Lock();
            break;

        case ROTATE:
            Rotate(nxt);

            // 踢墙  
            {
                int i = 0;
                while(!IsValid(nxt) && i < 4)
                {
                    mPosition = mBlocks[i++];
                    Rotate(nxt);
                }
            }
            mPosition = mBlocks[1];

            break;

        case HARD_DROP:
            for(int i = 0; i < 4; ++i) mBlocks[i] = mGhost[i];

            mBoard->Lock(mBlocks, mType);
            mBoard->ClearLines();
            Spawn();

            break;

        default: break;
    }
}

void Piece::Rotate(std::vector<Vector2>& out) const 
{
    for(int i = 0; i < 4; ++i)
    {
        int rx = mBlocks[i].x - mPosition.x;
        int ry = mBlocks[i].y - mPosition.y;
        out[i].x = mPosition.x - ry;
        out[i].y = mPosition.y + rx;
    }
}