#include "Board.h"
#include "Game.h"
#include<SDL2/SDL.h>

Board::Board(Game* game)
:Actor(game)
{
    Reset();
}



void Board::Reset()
{
    mGrid.assign(Config::BOARD_ROW, std::vector<int>(Config::BOARD_COLUMN, -1));
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
}

void Board::ClearLines()
{
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
        }
    }
}



void Board::Draw(SDL_Renderer* renderer)
{
    for(int y = 0; y < Config::BOARD_ROW; ++y)
    {
        for(int x = 0; x < Config::BOARD_COLUMN; ++x)
        {
            if(mGrid[y][x] == -1) continue;

            Color c = Config::COLORS[mGrid[y][x]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_Rect rc = {
                static_cast<int>(x * Config::BOARD_CELL + 1), 
                static_cast<int>(y * Config::BOARD_CELL + 1), 
                static_cast<int>(Config::BOARD_CELL - 2), 
                static_cast<int>(Config::BOARD_CELL - 2)
            };
            SDL_RenderFillRect(renderer, &rc);
        }
    }
}