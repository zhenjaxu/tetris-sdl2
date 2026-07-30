#pragma once
#include <vector>
#include <cstdint>

class Game{
public:
    Game();
    bool Initialize();   
    void RunLoop();      
    void Shutdown();    

    void AddActor(class Actor* actor);      
    void RemoveActor(class Actor* actor);   

    const std::vector<class Actor*>& GetActors() const { return mActors; }

private:
    void ProcessInput();   
    void UpdateGame();     
    void GenerateOutput(); 

    void LoadData();    
    void UnloadData();  

    class Renderer* mRenderer;
    uint32_t mTicksCount;
    bool mIsRunning;

    std::vector<class Actor*> mActors;

    class Piece* mPiece;
};