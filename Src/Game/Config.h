#pragma once
#include <string>
#include <cstdint>

struct RGBA
{
    uint8_t r, g, b, a;
};

struct XY
{
    float x, y;
};

struct Block
{
    bool blend;
    int x, y, w, h;
    RGBA color;
};

struct Text
{
    std::string content;
    int x, y;
    RGBA color;
};

namespace Config
{
    constexpr float TICK_RATE = 60.0f; 
    constexpr float TICK_DT = 1.0f / TICK_RATE;

    constexpr int BOARD_COLUMN = 10;
    constexpr int BOARD_ROW = 20;
    constexpr float BOARD_CELL = 30.0f;
    constexpr float BOARD_WIDTH = BOARD_COLUMN * BOARD_CELL;
    constexpr float BOARD_HEIGHT = BOARD_ROW * BOARD_CELL;
    constexpr float WINDOW_WIDTH = BOARD_COLUMN * BOARD_CELL + 200.0f;
    constexpr float WINDOW_HEIGHT = BOARD_ROW * BOARD_CELL;

    constexpr float DROP_SPEED = 60.0f;
    constexpr float SOFT_DROP = 300.0f;

    constexpr RGBA COLORS[7] = {
        {0,255,255,255}, {128,0,128,255}, {255,255,0,255}, {255,165,0,255}, 
        {0,0,255,255}, {0,255,0,255}, {255,0,0,255}
    };

    constexpr XY SHAPES[7][4] = {
        {{0,-1}, {0,0}, {0,1}, {0,2}},
        {{-1,0}, {0,0}, {1,0}, {0,1}},
        {{1,0}, {0,0}, {0,1}, {1,1}},
        {{0,-1}, {0,0}, {0,1}, {1,1}},
        {{0,-1}, {0,0}, {0,1}, {-1,1}},
        {{1,0}, {0,0}, {0,1}, {-1,1}},
        {{-1,0}, {0,0}, {0,1}, {1,1}}
    };

    constexpr int BASE_SCORE = 30;
    constexpr float COMBO_RAWORD = 1.5f;
};