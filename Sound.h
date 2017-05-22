#pragma once
#include "Filesystem.h"

#pragma region forward declarations
namespace FMOD
{
    class Channel;
    class Sound;
    class DSP;
}
#pragma endregion

namespace pk
{
    class Sound
    {
    public:
        Sound(void) = default;
        Sound(const std::filesystem::path& path, bool autoplay = true);
        Sound(const std::vector<float>& pcm, bool autoplay = true);

        FMOD::Channel* channel() const;

        void create(const std::filesystem::path& path, bool autoplay = true);
        void create(const std::vector<float>& pcm, bool autoplay = true);

        void play();

    private:
        FMOD::Channel* _channel;
        FMOD::Sound* _sound;
    };
}
