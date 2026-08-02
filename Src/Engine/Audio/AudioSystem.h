#pragma once
#include <unordered_map>
#include <string>

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
    class FMOD_SYSTEM* mSystem;
    class FMOD_SOUND* mBGM;
    class FMOD_CHANNEL* mChannel;
    std::unordered_map<std::string, class FMOD_SOUND*> mSFXs;
    
    class Game* mGame;
};