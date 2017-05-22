#include "FFT.h"
#include <fmod.hpp>

#include "logger.h"

#include "SoundSystem.h"
#include "Sound.h"

namespace pk
{
    FFT::FFT(FMOD::Channel& channel)
    {
        bind(channel);
    }
    FFT::~FFT(void)
    {
        unbind();
    }

    std::vector<float> FFT::values()
    {
        FMOD_DSP_PARAMETER_FFT* parameter;
        _dsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**) &parameter, 0, 0, 0);

        int length = parameter->length;

        std::vector<float> result(length);
        for(int channel = 0; channel < parameter->numchannels; ++channel)
        {
            for(int bin = 0; bin < length; ++bin)
            {
                result[bin] += parameter->spectrum[channel][bin];
            }
        }

        return result;
    }

    void FFT::bind(FMOD::Channel& channel)
    {
        unbind();

        auto result = SoundSystem::_system->createDSPByType(FMOD_DSP_TYPE::FMOD_DSP_TYPE_FFT, &_dsp);
        logger::assert_cond(result == FMOD_OK, "FFT", "ctor", "createDSPByType failed");

        _dsp->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 1024);
        _dsp->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HAMMING);

        _channel = &channel;
        _channel->addDSP(0, _dsp);
    }
    void FFT::unbind()
    {
        if(_channel != nullptr)
        {
            _channel->removeDSP(_dsp);
            _channel = nullptr;
        }
    }
}