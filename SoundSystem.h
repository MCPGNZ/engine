#pragma once

namespace FMOD
{
    class System;
}

namespace pk
{
    class SoundSystem
    {
    public:
        SoundSystem(void);
        ~SoundSystem(void);

        void update();

    private:
        static FMOD::System* _system;
        static const int _maxChannels = 36;

        friend class Sound;
        friend class FFT;
    };
}