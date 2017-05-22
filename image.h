#pragma once

#include <string>
#include <vector>

#include "vec2.h"
#include "color.h"
#include "Filesystem.h"

namespace pk
{
    class Image
    {
        template <typename T> using containter = std::vector<T>;
    private:
        containter<Color> _data;
        vec2i _size;

    public:
        #pragma region Ctors
        Image(void) = default;
        Image(const std::filesystem::path& path);
        Image(const vec2i n_size, const Color& c = Color::transparent);
        Image(const vec2i n_size, const containter<Color>& n_data);
        Image(const vec2i n_size, void* linear_data);
        Image(const vec2i n_size, void** data);
        Image(const containter<containter<Color>>& n_data);
        #pragma endregion

        #pragma region Public methods
        containter<Color>& data();
        const containter<Color>& data() const;

        Color* front();
        const Color* front() const;

        vec2i size() const;
        int bytes() const;

        Color get(const vec2i pixel) const;
        Color sample(const vec2f uv) const;

        bool valid(const vec2i point) const;

        void set(const vec2i pixel, const vec4f& Color);
        void set(const vec2i pixel, const Color& Color);
        void set(const containter<vec2i>& pixels, const Color& Color);

        void fill(const Color& Color);

        void create(const vec2i n_size);
        void create(const vec2i n_size, const containter<Color>& n_data);
        void create(const containter<containter<Color>>& n_data);

        void load(const std::filesystem::path& path);
        void save(const std::filesystem::path& path) const;

        void clear(const Color& c = Color::transparent);
        void clear(const Color& c, const Color::Channel channel);

        template <typename F>
        void process(const F& func);
        #pragma endregion
    };

    template <typename F>
    void Image::process(const F& func)
    {
        for(int y = 0; y < size().y; ++y)
        {
            for(int x = 0; x < size().x; ++x)
            {
                set(vec2i{x, y}, func(x, y));
            }
        }
    }
}
