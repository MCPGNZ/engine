#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_Image_write.h"
#include "stb_Image.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION

#include "logger.h"

namespace pk
{
    #pragma region Ctors
    Image::Image(const std::filesystem::path& path)
    {
        load(path);
    }
    Image::Image(const vec2i size, const Color& color /* = Color::transparent */)
    {
        create(size);
        clear(color);
    }

    Image::Image(const vec2i size, const containter<Color>& data) :
        _size{size}, _data{data}
    {}

    Image::Image(const vec2i size, void* linear_data) :
        _size{size}
    {
        _data.resize(size.x * size.y);

        uint32_t* data = reinterpret_cast<uint32_t*>(linear_data);
        for(int y = 0; y < size.y; ++y)
        {
            for(int x = 0; x < size.x; ++x)
            {
                const int index = y * size.x + x;

                auto entry = (data + index);
                _data[index] = *entry;
            }
        }
    }
    Image::Image(const vec2i size, void** data)
    {
        _size = size;
    }
    Image::Image(const containter<containter<Color>>& ndata)
    {
        create(ndata);
    }
    #pragma endregion

    #pragma region Public methods
    void Image::create(vec2i n_size)
    {
        _data.clear();

        _size = n_size;
        _data.resize(_size.width * _size.height);
    }
    void Image::create(const vec2i n_size, const containter<Color>& ndata)
    {
        _size = n_size;
        _data = ndata;
    }
    void Image::create(const containter<containter<Color>>& ndata)
    {
        _size = pk::vec2i{static_cast<int>(ndata.size()), static_cast<int>(ndata[0].size())};

        for(int y = 0; y < _size.y; ++y)
        {
            for(int x = 0; x < _size.x; ++x)
            {
                _data.emplace_back(ndata[x][y]);
            }
        }
    }

    Image::containter<Color>& Image::data()
    {
        return _data;
    }
    const Image::containter<Color>& Image::data() const
    {
        return _data;
    }

    Color* Image::front()
    {
        return _data.data();
    }
    const Color* Image::front() const
    {
        return _data.data();
    }

    vec2i Image::size() const
    {
        return _size;
    }
    int Image::bytes() const
    {
        return _size.x * _size.y * sizeof(Color);
    }

    void Image::load(const std::filesystem::path& path)
    {
        logger::assert_cond(std::filesystem::exists(path), "Image", "load", "file does not exists");

        _data.clear();

        int comp;
        auto stbidata = stbi_load(path.string().c_str(), &_size.width, &_size.height, &comp, 4);

        logger::assert_cond(stbidata != nullptr, "Image", "load", "failed to load a file");

        _data.resize(_size.width * _size.height);
        memcpy(_data.data(), stbidata, sizeof(Color) * _data.size());

        stbi_image_free(stbidata);
    }
    void Image::save(const std::filesystem::path& path) const
    {
        stbi_write_png(path.string().c_str(), _size.width, _size.height, 4, _data.data(), 0);
    }

    Color Image::get(const vec2i pixel) const
    {
        return _data[pixel.y* _size.width + pixel.x];
    }
    Color Image::sample(const vec2f uv) const
    {
        return get(static_cast<vec2i>(vec2f{uv.x * _size.x, uv.y * _size.y}));
    }

    bool Image::valid(const vec2i point) const
    {
        return point.x >= 0 && point.y >= 0 && point.x < _size.width && point.y < _size.height;
    }

    void Image::set(const vec2i pixel, const vec4f& Color)
    {
        _data[pixel.y * _size.width + pixel.x] = Color;
    }
    void Image::set(const vec2i pixel, const Color& Color)
    {
        _data[pixel.y * _size.width + pixel.x] = Color;
    }
    void Image::set(const containter<vec2i>& pixels, const Color& Color)
    {
        for(const auto pixel : pixels)
        {
            _data[pixel.y * _size.width + pixel.x] = Color;
        }
    }

    void Image::fill(const Color& Color)
    {
        for(auto& c : _data)
        {
            c = Color;
        }
    }

    void Image::clear(const Color& Color)
    {
        for(int i = 0; i < _size.x * _size.y; ++i)
        {
            _data[i] = Color;
        }
    }
    void Image::clear(const Color& Color, const Color::Channel channel)
    {
        for(int i = 0; i < _size.x * _size.y; ++i)
        {
            if(channel & Color::R) _data[i].r = Color.r;
            if(channel & Color::G) _data[i].g = Color.g;
            if(channel & Color::B) _data[i].b = Color.b;
            if(channel & Color::A) _data[i].a = Color.a;
        }
    }
    #pragma endregion
}