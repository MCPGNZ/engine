#include "Sound.h"

#include <fmod.hpp>
#include "logger.h"
#include "SoundSystem.h"
#include "AudioWAV.h"

namespace pk
{
    Sound::Sound(const std::filesystem::path & path, bool autoplay /* = true */)
    {
        create(path, autoplay);
    }

    Sound::Sound(const std::vector<float>& pcm, bool autoplay /* = true */)
    {
        create(pcm, autoplay);
    }

    FMOD::Channel * Sound::channel() const
    {
        return _channel;
    }

    void Sound::create(const std::filesystem::path & path, bool autoplay /* = true */)
    {
        logger::assert_cond(std::filesystem::exists(path), "Sound", "create(path)", "file does not exist");

        auto result = SoundSystem::_system->createSound(path.string().c_str(), FMOD_DEFAULT, nullptr, &_sound);
        logger::assert_cond(result == FMOD_OK, "Sound", "create(path)", "createSound failed");

        play();
    }

    void Sound::create(const std::vector<float>& pcm, bool autoplay /* = true */)
    {
        AudioWAV file{pcm};

        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.length = file.bytes();

        auto result =  SoundSystem::_system->createSound(file.data(), FMOD_OPENMEMORY, &exinfo, &_sound);
        logger::assert_cond(result == FMOD_OK, "Sound", "create(pcm)", "createSound");

        play();
    }

    void Sound::play()
    {
        logger::assert_cond(_sound != nullptr, "Sound", "play", "_sound == nullptr");

        auto result = _sound->setMode(FMOD_LOOP_NORMAL);
        logger::assert_cond(result == FMOD_OK, "Sound", "play", "setMode failed");

        result = _sound->setLoopCount(-1);
        logger::assert_cond(result == FMOD_OK, "Sound", "play", "setLoopCound failed");

        result = SoundSystem::_system->playSound(_sound, nullptr, false, &_channel);
        logger::assert_cond(result == FMOD_OK, "Sound", "play", "playSound failed");
    
    }
}