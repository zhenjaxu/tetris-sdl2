#include "Board.h"
#include "Game.h"
#include "BlockSpriteComponent.h"
#include "UI.h"
#include "AudioSystem.h"
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

    block.w = Config::BOARD_WIDTH;
    block.h = Config::BOARD_HEIGHT;
    block.x = -Config::WINDOW_WIDTH / 2;
    block.y = Config::WINDOW_HEIGHT / 2;
    block.color = RGBA{0.12f, 0.12f, 0.12f, 1.0f};
    blocks->push_back(block);

    block.w = Config::BOARD_CELL - 2;
    block.h = Config::BOARD_CELL - 2;

    for(int y = 0; y < Config::BOARD_ROW; ++y)
    {
        for(int x = 0; x < Config::BOARD_COLUMN; ++x)
        {
            if(mGrid[y][x] == -1) continue;

            block.color = Config::COLORS[mGrid[y][x]];
            Vector2 pos = ToPos(Vector2{(float)x, (float)y});
            block.x = pos.x + 1;
            block.y = pos.y - 1;
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

// 返回方块左上角顶点位置
Vector2 Board::ToPos(const Vector2& block)
{
    Vector2 pos;
    pos.x = -Config::WINDOW_WIDTH / 2.0f + block.x * Config::BOARD_CELL;
    pos.y = Config::WINDOW_HEIGHT / 2.0f - block.y * Config::BOARD_CELL;
    return pos;
}

Vector2 Board::ToGrid(const Vector2& pos)
{
    Vector2 grid;
    grid.x = Config::BOARD_COLUMN / 2 + std::floor((pos.x + 100.0f) / Config::BOARD_CELL); 
    grid.y = Config::BOARD_ROW / 2 - std::ceil(pos.y / Config::BOARD_CELL); 
    return grid;
}