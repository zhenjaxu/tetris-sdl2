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
        HARD_DROP
    };

    Piece(class Game* game, class Board* board);

    void UpdateActor(float deltaTime) override;
    std::shared_ptr<std::vector<Block>> DrawCall() override;

    void SendInput(MoveType move);
    void SetDropTime(float time){ mDropTime = time; }

private:
    void Spawn();
    void Lock(); 
    bool IsValid(const std::vector<Vector2>& blocks);   
    void CalculateGhost(std::vector<Vector2>& ghost) const;

    void Move(MoveType move);
    void Rotate(std::vector<Vector2>& out) const;

    std::vector<MoveType> mInputEvent;

    std::vector<Vector2> mBlocks;
    std::vector<Vector2> mGhost;
    Vector2 mPosition;
    int mType;
    
    float mDropTime;
    float mDropAccum;

    class Board* mBoard;
};