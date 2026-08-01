#pragma once
#include "Actor.h"
#include "Config.h"
#include <memory>
#include <vector>
#include <cstdint>

class UI : public Actor
{
public:
    UI(class Game* game);
    void Reset();
    void PlusScore(uint32_t score){ mScoreCount += score; }
    void MinusScore(uint32_t score){ mScoreCount -= score; }

    std::shared_ptr<std::vector<Text>> GetTexts();

private:
    uint32_t mLastScore;
    uint32_t mScoreCount;

    class Board* mBoard;
};