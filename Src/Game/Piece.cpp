#include "Piece.h"
#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "InputComponent.h"
#include "BlockSpriteComponent.h"
#include <ctime>
#include <SDL2/SDL.h>

Piece::Piece(Game* game, Board* board)
: Actor(game)
, mBoard(board)
, mBlocks(4)
, mGhost(4)
, mNext(4)
, mType(-1)
, mNextType(-1)
, mDropSpeed(0.0f)
{
    auto ic = new InputComponent(this);
    auto bsc = new BlockSpriteComponent(this);

    srand((unsigned)time(nullptr));
    Spawn();
}

void Piece::UpdateActor(float deltaTime)
{
    for(auto input : mInputEvent)
    {
        Move(input);
        CalculateGhost(mGhost);
    }
    mInputEvent.clear();

    Vector2 prev = Math::ToGrid(mPosition);
    mPosition.y += mDropSpeed * deltaTime;
    Vector2 curr = Math::ToGrid(mPosition);
    if(curr.y != prev.y) Move(DROP);
}

std::shared_ptr<std::vector<Block>> Piece::GetBlocks()
{
    auto blocks = std::make_shared<std::vector<Block>>();

    // 显示即将生成的方块
    Color c = Config::COLORS[mNextType];
    Block block;
    block.w = Config::BOARD_CELL - 2;
    block.h = Config::BOARD_CELL - 2;
    for(int i = 0; i < 4; ++i)
    {
        block.blend = false;
        block.x = mNext[i].x * Config::BOARD_CELL + 1;
        block.y = mNext[i].y * Config::BOARD_CELL + 1;
        block.color = c;
        blocks->push_back(block);
    }

    c = Config::COLORS[mType];
    for(int i = 0; i < 4; ++i)
    {
        if(mGhost[i].y < 0) continue;

        block.blend = true;
        c.a = 80;
        block.x = mGhost[i].x * Config::BOARD_CELL + 1;
        block.y = mGhost[i].y * Config::BOARD_CELL + 1;
        block.color = c;
        blocks->push_back(block);

        if(mBlocks[i].y < 0) continue;
        
        block.blend = false;
        c.a = 255;
        block.x = mBlocks[i].x * Config::BOARD_CELL + 1;
        block.y = mBlocks[i].y * Config::BOARD_CELL + 1;
        block.color = c;
        blocks->push_back(block);
    }

    return blocks;
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
    if(mType == -1) mType = rand() % 7;
    else mType = mNextType;
    mNextType = rand() % 7;

    // 即将生成的方块
    Vector2 NextPos;
    NextPos.x = Config::BOARD_COLUMN + 3;
    NextPos.y = 3;
    for(int i = 0; i < 4; ++i)
    {
        mNext[i].x = Config::SHAPES[mNextType][i].x + NextPos.x;
        mNext[i].y = Config::SHAPES[mNextType][i].y + NextPos.y;
    }

    mPosition.x = Config::BOARD_WIDTH / 2;
    mPosition.y = Config::BOARD_CELL;
    Vector2 center = Math::ToGrid(mPosition);
    for(int i = 0; i < 4; ++i)
    {
        mBlocks[i].x = Config::SHAPES[mType][i].x + center.x;
        mBlocks[i].y = Config::SHAPES[mType][i].y + center.y;
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
            if(IsValid(nxt)) mPosition.x -= Config::BOARD_CELL;
            break;

        case RIGHT:
            for(int i = 0; i < 4; ++i) nxt[i] = {mBlocks[i].x + 1, mBlocks[i].y};
            if(IsValid(nxt)) mPosition.x += Config::BOARD_CELL;
            break;

        case DROP:
            for(int i = 0; i < 4; ++i) nxt[i] = {mBlocks[i].x, mBlocks[i].y + 1};
            if(!IsValid(nxt)) Lock();
            break;

        case ROTATE: 
            // 踢墙  
            {
                Vector2 center = Math::ToGrid(mPosition);
                Rotate(nxt, center);

                int i = 0;
                while(!IsValid(nxt) && i < 4)
                {
                    center = mBlocks[i++];
                    Rotate(nxt, center);
                }
            }

            break;

        case HARD_DROP:
            for(int i = 0; i < 4; ++i) mBlocks[i] = mGhost[i];
            Lock();
            break;

        default: break;
    }
}

void Piece::Rotate(std::vector<Vector2>& out, const Vector2& center) const 
{
    for(int i = 0; i < 4; ++i)
    {
        int rx = mBlocks[i].x - center.x;
        int ry = mBlocks[i].y - center.y;
        out[i].x = center.x - ry;
        out[i].y = center.y + rx;
    }
}