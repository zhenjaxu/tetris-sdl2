#include "UI.h"
#include "Game.h"
#include "Board.h"
#include "TextSpriteComponent.h"
#include <string>

UI::UI(Game* game)
: Actor(game)
, mLastScore(0)
, mScoreCount(0)
{
    auto tsc = new TextSpriteComponent(this);
}

void UI::Reset()
{
    mLastScore = mScoreCount;
    mScoreCount = 0;
}

std::shared_ptr<std::vector<Text>> UI::GetTexts()
{
    auto texts = std::make_shared<std::vector<Text>>();

    Text text;
    text.color = Color{255, 255, 255, 255};

    // 文字
    text.x = Config::BOARD_WIDTH + 10;

    text.content = "Next piece: ";
    text.y = static_cast<int>(Config::BOARD_CELL * 1);
    texts->push_back(text);

    text.content = "Last score: ";
    text.y = static_cast<int>(Config::BOARD_CELL * 6);
    texts->push_back(text);

    text.content = "Current score: ";
    text.y = static_cast<int>(Config::BOARD_CELL * 8);
    texts->push_back(text);

    // 数字
    text.x = Config::BOARD_WIDTH + 20;

    text.content = std::to_string(mLastScore);
    text.y = static_cast<int>(Config::BOARD_CELL * 7);
    texts->push_back(text);

    text.content = std::to_string(mScoreCount);
    text.y = static_cast<int>(Config::BOARD_CELL * 9);
    texts->push_back(text);

    return texts;
}