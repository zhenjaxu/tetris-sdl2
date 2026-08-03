#pragma once
#include <unordered_map>
#include <string>
#include <fmod.h>

class AudioSystem
{
public:
    AudioSystem(class Game* game);
    bool Initialize();
    void UnloadData();
    void Shutdown();

    void Update();
    void PlayBGM();
    void PlaySFX(const std::string& fileName, float volume = 0.8f);
    void LoadSound(const std::string& fileName);

private:
    FMOD_SYSTEM* mSystem;
    FMOD_SOUND* mBGM;
    FMOD_CHANNEL* mChannel;
    std::unordered_map<std::string, FMOD_SOUND*> mSFXs;
    
    class Game* mGame;
};