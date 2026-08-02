#include "Board.h"
#include "Game.h"
#include "Config.h"
#include "BlockSpriteComponent.h"
#include "UI.h"
#include "AudioSystem.h"
#include <SDL2/SDL.h>
#include <cmath>

Board::Board(Game* game)
: Actor(game)
{
    auto bsc = new BlockSpriteComponent(this);
    mScoreCounter = new UI(game);

    Reset();
}



std::shared_ptr<std::vector<Block>> Board::GetBlocks()
{
    auto blocks = std::make_shared<std::vector<Block>>();
    Block block;
    block.blend = false;
    block.w = Config::BOARD_CELL - 2;
    block.h = Config::BOARD_CELL - 2;

    for(int y = 0; y < Config::BOARD_ROW; ++y)
    {
        for(int x = 0; x < Config::BOARD_COLUMN; ++x)
        {
            if(mGrid[y][x] == -1) continue;

            block.color = Config::COLORS[mGrid[y][x]];
            block.x = x * Config::BOARD_CELL + 1;
            block.y = y * Config::BOARD_CELL + 1;
            blocks->push_back(block);
        }
    }

    return blocks;
}



void Board::Reset()
{
    mGrid.assign(Config::BOARD_ROW, std::vector<int>(Config::BOARD_COLUMN, -1));
    if(mScoreCounter) mScoreCounter->Reset();
    GetGame()->GetAudioSystem()->PlayBGM();
}

bool Board::IsValid(const std::vector<Vector2>& blocks) const 
{
    for(int i = 0; i < 4; ++i)
    {
        if(blocks[i].x < 0 ||
           blocks[i].x >= Config::BOARD_COLUMN ||
           blocks[i].y >= Config::BOARD_ROW) 
        {
            return false;
        }

        if(blocks[i].y >= 0 &&
           mGrid[blocks[i].y][blocks[i].x] != -1)
        {
            return false;
        }
    }
    return true;
}

void Board::Lock(const std::vector<Vector2>& blocks, int type)
{
    for(int i = 0; i < 4; ++i)
    {
        if(blocks[i].y >= 0) mGrid[blocks[i].y][blocks[i].x] = type;
    }
    GetGame()->GetAudioSystem()->PlaySFX("Assets/success.wav", 1.0f);
}

void Board::ClearLines()
{
    int lines = 0;

    for(int y = Config::BOARD_ROW - 1; y >= 0; --y)
    {
        bool full = true;
        for(int x = 0; x < Config::BOARD_COLUMN; ++x)
        {
            if(mGrid[y][x] == -1)
            {
                full = false;
                break;
            }
        }

        if(full)
        {
            mGrid.erase(mGrid.begin() + y);
            mGrid.insert(mGrid.begin(), std::vector<int>(Config::BOARD_COLUMN, -1));
            ++y;

            ++lines;
        }
    }

    mScoreCounter->PlusScore(lines * (int)(Config::BASE_SCORE * std::pow(Config::COMBO_RAWORD, lines - 1)));

    if(lines != 0) GetGame()->GetAudioSystem()->PlaySFX("Assets/right.wav", 1.0f);
}