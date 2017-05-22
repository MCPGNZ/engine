#pragma once
#include <vector>

#pragma region forward declarations
namespace FMOD
{
    class DSP;
    class Channel;
}
#pragma endregion 

namespace pk
{
    class Sound;

    class FFT
    {
    public:
        FFT(void) = default;
        FFT(FMOD::Channel& channel);
        ~FFT(void);

        std::vector<float> values();

        void bind(FMOD::Channel& channel);
        void unbind();

    private:
        FMOD::DSP* _dsp = nullptr;
        FMOD::Channel* _channel = nullptr;
    };
}

