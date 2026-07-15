#include"MoveComponent.h"
#include"Actor.h"
#include"Command.h"
#include"Game.h"
#include"InputMananger.h"
#include"BlocksComponent.h"
#include"Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
:Component(owner, updateOrder)
{}

void MoveComponent::Update(float updateTime){
    auto input=mOwner->GetGame()->GetInput();
    mMoveDir=0;
    if(input->IsHeld(SDL_SCANCODE_A)) mMoveDir-=1;
    if(input->IsHeld(SDL_SCANCODE_D)) mMoveDir+=1;
    mNextDrop=input->IsHeld(SDL_SCANCODE_S)?SOFTDROP:DROP;

    mBlocks=mOnwer->GetComponent<BlocksComponent>();
    if(!mBlocks) return;
    
    // 执行命令
    for(auto& command:mCommands){
        command->Execute(mOwner);
    }
    mCommands.clear();
    
    // DAS延迟
    if(mMoveDir){
        if(mDASTimer==0.0f){
            Move();
        }else{
            if(mDASTimer>=DAS_DELAY){
                mARRTimer+=updateTime;
                if(mARRTimer>=ARR_INTERVAL){
                    Move();
                    mARRTimer=0.0f;
                }
            }
        }
        mDASTimer+=updateTime;
    }else{
        mDASTimer=0.0f;
        mARRTimer=0.0f;
    }

    mDropTimer+=updateTime;
    if(mDropTimer>=mNextDrop){
        Drop();
        mDropTimer=0.0f;
    }
}

void MoveComponent::PushCommand(std::unique_ptr<Command> command){
    mCommands.push_back(std::move(command));
}

// 水平移动
void MoveComponent::Move(){
    Vector2 blocks[4]=mBlocks->GetBlocks();
    Vector2 nxt[4];
    Vector2 pos=mOwner->GetPosition();

    for(int i=0;i<4;++i) nxt[i]=Vector2(mMoveDir+blocks[i].x, blocks[i].y);

    mBlocks->SetBlocks(nxt);
    pos.x+=mMoveDir;

    // 回退
    if(!mBlocks->IsValid()){
        mBlocks->SetBlocks(blocks);     
        pos.x-=mMoveDir;
    }

    mOwner->SetPosition(pos);
}

// 下落
void MoveComponent::Drop(){
    Vector2 blocks[4]=mBlocks->GetBlocks();
    Vector2 nxt[4];
    Vector2 pos=mOwner->GetPosition();

    for(int i=0;i<4;++i) nxt[i]=Vector2(blocks[i].x, blocks[i].y+1);

    mBlocks->SetBlocks(nxt);
    pos.y+=1;

    if(!mBlocks->IsValid()){
        mBlocks->SetBlocks(blocks);     
        mBlocks->Lock();
        break;
    }

    mOwner->SetPosition(pos);
}

// 旋转
void MoveComponent::Rotate(){
    Vector2 blocks[4]=mBlocks->GetBlocks();
    Vector2 nxt[4];
    Vector2 pos=mOwner->GetPosition();

    // 绕中心顺时针旋转90度
    for(int i=0;i<4;++i){
        float rx=blocks[i].x-pos.x;
        float ry=blocks[i].y-pos.y;
        nxt[i].x=pos.x-ry;
        nxt[i].y=pos.y+rx;
    }

    if(!mBlocks->IsValid()){
        mBlocks->SetBlocks(blocks);
    }
}

// 硬降
void MoveComponent::HardDrop(){
    Vector2 ghost[4]=mBlocks->GetGhost();
    mBlocks->SetBlocks(ghost);
    mBlocks->Lock();
}