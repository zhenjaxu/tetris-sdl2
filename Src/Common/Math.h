#pragma once
#include "Config.h"
#include <cmath>

namespace Math
{
    constexpr float Pi = M_PI;
    constexpr float TwoPi = M_PI * 2;
    constexpr float PiOverTwo = M_PI / 2;

    inline Vector2 ToGrid(const Vector2& pos)
    {
        Vector2 grid;
        grid.x = static_cast<int>(pos.x / Config::BOARD_CELL); 
        grid.y = static_cast<int>(pos.y / Config::BOARD_CELL); 
        return grid;
    }

    constexpr float ToDegrees(float radian)
    {
        return radian * 180.0f / Pi;
    }
};
