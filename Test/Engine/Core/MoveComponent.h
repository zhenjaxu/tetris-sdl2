#pragma once
#include"Component.h"
#include<memory>
#include<vector>

class MoveComponent:public Component{
public:
    MoveComponent(class Actor* owner, int updateOrder=10);
    void Update(float updateTime) override;
    void PushCommand(std::unique_ptr<class Command> command);

private:
    void Move();
    void Drop();
    void Rotate();
    void HardDrop();

    std::vector<std::unique_ptr<class Command>> mCommands;
    class BlocksComponent* mBlocks;

    int mMoveDir;
    float mNextDrop;
    float mDropTimer;
    float mDASTimer;
    float mARRTimer;

    const float DAS_DELAY=0.5f;
    const float ARR_INTERVAL=0.2f;
    const float SOFTDROP=0.1f;
    const float DROP=0.5f;
};