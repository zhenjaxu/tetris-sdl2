#pragma once
#include "Actor.h"
#include "Config.h"
#include <vector>
#include <memory>

class Piece : public Actor{
public:
    enum MoveType : uint8_t
    {
        LEFT,
        RIGHT,
        DROP,
        ROTATE,
        HARD_DROP,
        RESET
    };

    Piece(class Game* game, class Board* board);
    void UpdateActor(float deltaTime) override;

    void SendInput(MoveType move);
    void SetDropSpeed(float speed){ mDropSpeed = speed; }
    
    void Spawn();

    std::shared_ptr<std::vector<Block>> GetBlocks();

private:
    void Lock(); 
    bool IsValid(const std::vector<Vector2>& blocks);   
    void CalculateGhost(std::vector<Vector2>& ghost) const;

    void Move(MoveType move);
    void Rotate(std::vector<Vector2>& out, const Vector2& center) const;
    void MoveSFX(const std::vector<Vector2>& nxt);

    std::vector<MoveType> mInputEvent;

    std::vector<Vector2> mBlocks;
    std::vector<Vector2> mGhost;
    int mType;

    // 即将生成的方块
    std::vector<Vector2> mNext;
    int mNextType;

    float mDropSpeed;

    class Board* mBoard;
};