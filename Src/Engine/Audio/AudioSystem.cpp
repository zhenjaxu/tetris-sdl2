#include "AudioSystem.h"
#include "Game.h"
#include <SDL2/SDL.h>
#include <fmod.h>
#include <fmod_errors.h>

AudioSystem::AudioSystem(Game* game)
: mGame(game)
, mSystem(nullptr)
, mBGM(nullptr)
, mChannel(nullptr)
{}

bool AudioSystem::Initialize()
{
    FMOD_RESULT result = FMOD_System_Create(&mSystem);
    if(result != FMOD_OK)
    {
        SDL_Log("Unable to creat audio system: %s", FMOD_ErrorString(result));
        return false;
    }

    result = FMOD_System_Init(mSystem, 32, FMOD_INIT_NORMAL, nullptr);
    if(result != FMOD_OK)
    {
        SDL_Log("Failed to initialize audio system: %s", FMOD_ErrorString(result));
        return false;
    }

    result = FMOD_System_CreateStream(mSystem, "Assets/BGM.wav", FMOD_LOOP_NORMAL, nullptr, &mBGM);
    if(result != FMOD_OK)
    {
        SDL_Log("Failed to create bgm: %s", FMOD_ErrorString(result));
        return false;
    }
    
    return true;
}

void AudioSystem::UnloadData()
{
    if(mBGM) FMOD_Sound_Release(mBGM);

    for(auto [name, sound] : mSFXs)
    {
        FMOD_Sound_Release(sound);
    }
}

void AudioSystem::Shutdown()
{
    FMOD_System_Close(mSystem);
    FMOD_System_Release(mSystem);
}

void AudioSystem::PlayBGM()
{
    FMOD_System_PlaySound(mSystem, mBGM, nullptr, false, &mChannel);
    FMOD_Channel_SetVolume(mChannel, 0.6f);
}

void AudioSystem::PlaySFX(const std::string& fileName, float volume)
{
    if(!mSFXs.count(fileName))
    {
        LoadSound(fileName);
    }

    FMOD_CHANNEL* channel = nullptr;
    FMOD_System_PlaySound(mSystem, mSFXs[fileName], nullptr, false, &channel);
    FMOD_Channel_SetVolume(channel, volume);
}

void AudioSystem::Update()
{
    FMOD_System_Update(mSystem);
}

void AudioSystem::LoadSound(const std::string& fileName)
{
    if(mSFXs.count(fileName)) return;

    FMOD_SOUND* sound = nullptr;
    FMOD_RESULT result = FMOD_System_CreateSound(mSystem, fileName.c_str(), FMOD_DEFAULT, nullptr, &sound);
    if(result != FMOD_OK)
    {
        SDL_Log("Failed to create sfx: %s", FMOD_ErrorString(result));
        return;
    }

    mSFXs.emplace(fileName, sound);
}