#pragma once

class AudioSystem
{
public:
    AudioSystem(class Game* game);
    bool Initialize();
    void UnloadData();
    void Shutdown();

    void PlayBGM();

private:
    class FMOD_SYSTEM* mSystem;
    class FMOD_SOUND* mBGM;
    class FMOD_CHANNEL* mChannel;
    
    class Game* mGame;
};