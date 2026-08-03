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

    class Renderer* GetRenderer() const { return mRenderer; }
    class AudioSystem* GetAudioSystem() const { return mAudioSystem; }

private:
    void ProcessInput();   
    void UpdateGame();     
    void GenerateOutput(); 

    void LoadData();    
    void UnloadData();  

    class Renderer* mRenderer;
    uint32_t mTicksCount;
    bool mIsRunning;

    bool mUpdatingActors;
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    class AudioSystem* mAudioSystem;
};