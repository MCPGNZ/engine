#pragma once
#include <vector>
#include "Filesystem.h"

namespace pk
{
    class AudioWAV
    {
    public:
        AudioWAV(void) = default;
        AudioWAV(const std::vector<float>& pcm);

        size_t bytes() const;
        char* data();

        void create(const std::vector<float>& pcm);
        void save(const std::filesystem::path& path) const;

    private:
        std::vector<char> _data;

        template <typename T>
        void add(T value);

        void add(const std::string& value);

        template <typename T>
        void set(int position, T value);
    };


    template<typename T>
    inline void AudioWAV::add(T value)
    {
        int size = sizeof(T);
        for(; size; --size, value >>= 8)
        {
            _data.push_back(static_cast<char>(value & 0xFF));
        }
    }

    template <typename T>
    inline void AudioWAV::set(int position, T value)
    {
        int size = sizeof(T);
        for(int i = 0; size; --size, ++i, value >>= 8)
        {
           _data[position + 1] = static_cast<char>(value & 0xFF);
        }
    }
}
