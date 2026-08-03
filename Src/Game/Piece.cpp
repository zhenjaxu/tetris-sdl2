#include "Piece.h"
#include "Board.h"
#include "Game.h"
#include "InputComponent.h"
#include "BlockSpriteComponent.h"
#include "AudioSystem.h"
#include "Random.h"

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

    Vector2 prev = Board::ToGrid(mPosition);
    mPosition.y -= mDropSpeed * deltaTime;
    Vector2 curr = Board::ToGrid(mPosition);
    if(curr.y != prev.y) Move(DROP);
}

std::shared_ptr<std::vector<Block>> Piece::GetBlocks()
{
    auto blocks = std::make_shared<std::vector<Block>>();

    Block block;
    Vector2 pos;
    block.w = Config::BOARD_CELL - 2;
    block.h = Config::BOARD_CELL - 2;

    // 显示即将生成的方块
    RGBA c = Config::COLORS[mNextType];
    for(int i = 0; i < 4; ++i)
    {
        block.blend = false;
        pos = Board::ToPos(mNext[i]);
        block.x = pos.x + 1; 
        block.y = pos.y - 1;
        block.color = c;
        blocks->push_back(block);
    }

    c = Config::COLORS[mType];
    for(int i = 0; i < 4; ++i)
    {
        if(mGhost[i].y < 0) continue;

        block.blend = true;
        c.a = 0.3f;
        pos = Board::ToPos(mGhost[i]);
        block.x = pos.x + 1;
        block.y = pos.y - 1;
        block.color = c;
        blocks->push_back(block);

        if(mBlocks[i].y < 0) continue;
        
        block.blend = false;
        c.a = 1.0f;
        pos = Board::ToPos(mBlocks[i]);
        block.x = pos.x + 1;
        block.y = pos.y - 1;
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

void Piece::Spawn()
{ 
    if(mType == -1) mType = Random::GetIntRange(0, 6);
    else mType = mNextType;
    mNextType = Random::GetIntRange(0, 6);

    // 即将生成的方块
    Vector2 NextPos{Config::BOARD_COLUMN + 3.0f, 3.0f};
    for(int i = 0; i < 4; ++i)
    {
        mNext[i].x = Config::SHAPES[mNextType][i].x + NextPos.x;
        mNext[i].y = Config::SHAPES[mNextType][i].y + NextPos.y;
    }

    Vector2 center{Config::BOARD_COLUMN / 2.0f, 1.0f};
    mPosition = Board::ToPos(center);
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
        for(int i = 0; i < 4; ++i) nxt[i]= Vector2{ghost[i].x, ghost[i].y + 1};
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
            for(int i = 0; i < 4; ++i) nxt[i] = Vector2{mBlocks[i].x - 1, mBlocks[i].y};
            if(IsValid(nxt)) mPosition.x -= Config::BOARD_CELL;
            MoveSFX(nxt);
            break;

        case RIGHT:
            for(int i = 0; i < 4; ++i) nxt[i] = Vector2{mBlocks[i].x + 1, mBlocks[i].y};
            if(IsValid(nxt)) mPosition.x += Config::BOARD_CELL;
            MoveSFX(nxt);
            break;

        case DROP:
            for(int i = 0; i < 4; ++i) nxt[i] = Vector2{mBlocks[i].x, mBlocks[i].y + 1};
            if(!IsValid(nxt)) Lock();
            break;

        case ROTATE: 
            // 踢墙  
            {
                Vector2 center = Board::ToGrid(mPosition);
                for(int i = 0; i < 4; ++i) nxt[i] = Vector2{mBlocks[i].x, mBlocks[i].y};
                Rotate(nxt, center);

                if(!IsValid(nxt))
                {
                    for(auto kick : Config::WALL_KICK)
                    {
                        Vector2 k(kick.x, kick.y);
                        for(int i = 0; i < 4; ++i) nxt[i] = mBlocks[i] + k;
                        Rotate(nxt, center + k);

                        if(IsValid(nxt)){
                            mPosition.x += k.x * Config::BOARD_CELL;
                            mPosition.y -= k.y * Config::BOARD_CELL;
                            break;
                        }
                    }
                }

                MoveSFX(nxt);
            }

            break;

        case HARD_DROP:
            for(int i = 0; i < 4; ++i) mBlocks[i] = mGhost[i];
            Lock();
            break;

        case RESET:
            mBoard->Reset();
            Spawn();
            break;

        default: break;
    }
}

void Piece::Rotate(std::vector<Vector2>& nxt, const Vector2& center) const 
{
    for(int i = 0; i < 4; ++i)
    {
        float rx = nxt[i].x - center.x;
        float ry = nxt[i].y - center.y;
        nxt[i].x = center.x - ry;
        nxt[i].y = center.y + rx;
    }
}

void Piece::MoveSFX(const std::vector<Vector2>& nxt)
{
    bool success = true;
    for(int i = 0; i < 4; ++i)
    {
        if(nxt[i].x != mBlocks[i].x || nxt[i].y != mBlocks[i].y)
        {
            success = false;
            break;
        }
    }
    if(success) GetGame()->GetAudioSystem()->PlaySFX("Assets/dong.wav", 0.4f);
    else GetGame()->GetAudioSystem()->PlaySFX("Assets/error.wav", 1.0f);
}