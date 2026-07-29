#pragma once

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

    constexpr float DROP_TIME = 0.5f;
    constexpr float SOFT_DROP = 0.1f;
};