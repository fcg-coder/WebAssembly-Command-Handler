#pragma once
#include <cstdint>
#include <cstdio>

#ifdef USE_WEBASM
#endif

using uint = unsigned int;
/**
 * @brief pixels as RGBA: 0xRRGGBBAA format
 *
 */
struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : red(r), green(g), blue(b), alpha(a) {}

    // Basic colors
    static constexpr Color Black() { return {0, 0, 0}; }
    static constexpr Color White() { return {255, 255, 255}; }
    static constexpr Color Red() { return {255, 0, 0}; }
    static constexpr Color Green() { return {0, 255, 0}; }
    static constexpr Color Blue() { return {0, 0, 255}; }
    static constexpr Color Yellow() { return {255, 255, 0}; }
    static constexpr Color Cyan() { return {0, 255, 255}; }
    static constexpr Color Magenta() { return {255, 0, 255}; }
    static constexpr Color Gray() { return {128, 128, 128}; }

    // Transparent colors
    static constexpr Color Transparent() { return {0, 0, 0, 0}; }
    static constexpr Color SemiRed() { return {255, 0, 0, 128}; }
    static constexpr Color SemiGreen() { return {0, 255, 0, 128}; }
    static constexpr Color SemiBlue() { return {0, 0, 255, 128}; }

    // UI colors
    static constexpr Color DarkRed() { return {139, 0, 0}; }
    static constexpr Color DarkGreen() { return {0, 100, 0}; }
    static constexpr Color DarkBlue() { return {0, 0, 139}; }
    static constexpr Color LightGray() { return {211, 211, 211}; }
    static constexpr Color Orange() { return {255, 165, 0}; }
    static constexpr Color Pink() { return {255, 192, 203}; }
    static constexpr Color Purple() { return {128, 0, 128}; }
    static constexpr Color Brown() { return {165, 42, 42}; }
};

class Pixel
{
public:
    uint x;
    uint y;
    Color m_color;
    /**
     * @todo
     *
     */
    Pixel()
        : Pixel(0, 0, 0, 0, 0, 255)
    {
    }
    Pixel(uint x, uint y, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : x(x), y(y), m_color(red, green, blue, alpha) {}

    explicit Pixel(uint x, uint y, const Color& color)
        : x(x), y(y), m_color(color) {}

    uint32_t serialize() const
    {
        return (static_cast<uint32_t>(m_color.red) << 24) |
               (static_cast<uint32_t>(m_color.green) << 16) |
               (static_cast<uint32_t>(m_color.blue) << 8) |
               (static_cast<uint32_t>(m_color.alpha));
    }

    void deserialize(uint32_t rgba)
    {
        m_color.red = (rgba >> 24) & 0xFF;
        m_color.green = (rgba >> 16) & 0xFF;
        m_color.blue = (rgba >> 8) & 0xFF;
        m_color.alpha = rgba & 0xFF;
    }
};
