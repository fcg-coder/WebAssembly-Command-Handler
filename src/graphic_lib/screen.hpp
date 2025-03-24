#pragma once
#include <cstdint>
#include <cstdio>
#include "../main.hpp"
#include <random>
#define MAX_WIDTH  1920
#define MAX_HEIGHT 1080

#define MAX_SIZE (MAX_HEIGHT * MAX_WIDTH)

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

    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : red(red), green(green), blue(blue), alpha(alpha) {}
};

/**
 * @brief Pixels class
 *
 * @param color (rgba format)
 * @param coordinate (x,y)
 *
 *      0,0-----------------------> x  MAX_WIDTH
 *      |
 *      |
 *      |
 *      |
 *      |
 *   y  V
 *
 *  MAX_HEIGHT
 *
 */
class Pixel
{
private:
    int x; // abscissa
    int y; // ordinate
    Color m_color;

public:
    Pixel(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : m_color(red, green, blue, alpha) {}

    uint32_t serialize() const
    {
        return (static_cast<uint32_t>(m_color.red) << 24) |
               (static_cast<uint32_t>(m_color.green) << 16) |
               (static_cast<uint32_t>(m_color.blue) << 8) |
               (static_cast<uint32_t>(m_color.alpha));
    }

    //  uint32_t (0xRRGGBBAA)
    void deserialize(uint32_t rgba)
    {
        m_color.red = (rgba >> 24) & 0xFF;
        m_color.green = (rgba >> 16) & 0xFF;
        m_color.blue = (rgba >> 8) & 0xFF;
        m_color.alpha = rgba & 0xFF;
    }
};

/**
 * @brief Screen
 *
 * screenBuff is array that contains pixels as RGBA: 0xRRGGBBAA format
 *
 *
 *  @return MAX_SIZE screenBuff, but filled current SIZE
 */
class Screen
{
private:
    Screen() = default;
    static class Screen* m_instance;

    /**
     * @brief Size in pixels
     *
     */
    static int m_windowHeight;
    static int m_windowWidth;

    /***
     * @brief
     *
     *
     * T O - D O :
     *  - SIMD render
     */
    void render()
    {
        const int width = m_windowWidth;
        const int height = m_windowHeight;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dither(-0.5f, 0.5f);

        for (int y = 0; y < height; y++)
        {

            for (int x = 0; x < width; x++)
            {

                float t = static_cast<float>(x + y) / (width + height);
                uint8_t red = static_cast<uint8_t>(t * 255);
                uint8_t green = static_cast<uint8_t>((1.0f - t) * 255);
                Pixel pixel(static_cast<uint8_t>(t * 255), static_cast<uint8_t>(t * 255), static_cast<uint8_t>(t * 255), static_cast<uint8_t>(t * 255));

                screenBuff[y * width + x] = pixel.serialize();
            }
        }
    }

public:
    uint32_t screenBuff[MAX_SIZE];
    static Screen* getInstance()
    {
        if (! m_instance)
        {
            m_instance = new Screen();
        }
        return m_instance;
    }
    uint32_t* getScreen()
    {
        render();
        return screenBuff;
    }

    void setSize(int h, int w);
};