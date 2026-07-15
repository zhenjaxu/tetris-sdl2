#pragma once
#include"Component.h"
#include"Math.h"

class BlocksComponent:public Component{
public:
    BlocksComponent(class Actor* mOwner);

private:
    Vector2 mBlocks[4];
    
};