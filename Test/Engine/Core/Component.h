#pragma once

class Component{
public:
    Component(class Actor* owner, int updateOrder=100);
    ~Component();

    virtual void Update(float updateTime);

    int GetUpdateOrder() const {return mUpdateOrder;}

protected:
    class Actor* mOwner;
    int mUpdateOrder;
};