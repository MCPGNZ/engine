#include "AudioWAV.h"

#include <stdint.h>
#include <fstream>

namespace pk
{
    #pragma region ctors/dtors
    AudioWAV::AudioWAV(const std::vector<float>& pcm)
    {
        create(pcm);
    }
    #pragma endregion 

    #pragma region public methods
    size_t AudioWAV::bytes() const
    {
        return _data.size();
    }
    char* AudioWAV::data()
    {
        return _data.data();
    }

    void AudioWAV::create(const std::vector<float>& pcm)
    {
        _data.clear();

        // header
        add(std::string("RIFF----WAVEfmt "));
        add(int32_t{16});
        add(int16_t{1});
        add(int16_t{2});
        add(int32_t{44100});
        add(int32_t{176400});
        add(int16_t{4});
        add(int16_t{16});

        // data
        const int data_chunk_pos = bytes();
        add(std::string("data----"));

        const double two_pi = 6.283185307179586476925286766559;
        const double max_amplitude = 32760.0;

        for(const auto& sample : pcm)
        {
            const auto value = (sample - 0.5f) * max_amplitude;

            add(static_cast<int16_t>(max_amplitude * value));
            add(static_cast<int16_t>(max_amplitude * value));
        }

        const auto file_length = bytes();

        set(data_chunk_pos + 4, static_cast<int32_t>(file_length - data_chunk_pos + 8));
        set(4, static_cast<int32_t>(file_length - 8));
    }
    #pragma endregion 

    #pragma region private methods
    void AudioWAV::add(const std::string& value)
    {
        const auto length = value.length();
        for(size_t i = 0; i < length; ++i)
        {
            _data.push_back(value[i]);
        }
    }

    void AudioWAV::save(const std::filesystem::path& path) const
    {
        std::ofstream file{path};
        for(const auto& entry : _data)
        {
            file << entry;
        }
        file.close();
    }
    #pragma endregion 
}
