#pragma once
#include <cstdint>
#include <cstdio>
#include "../main.hpp"

#define MAX_WIDTH 1920
#define MAX_HEIGH 1080

#define MAX_SIZE (MAX_WIDTH * MAX_WIDTH)

/**
 * @brief pixels as RGBA: 0xRRGGBBAA format
 *
 */
class Pixel
{
private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
    uint8_t m_alpha;

public:
    Pixel(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) {}

    uint32_t serialize() const
    {
        return (static_cast<uint32_t>(m_red) << 24) |
               (static_cast<uint32_t>(m_green) << 16) |
               (static_cast<uint32_t>(m_blue) << 8) |
               (static_cast<uint32_t>(m_alpha));
    }

    //  uint32_t (0xRRGGBBAA)
    void deserialize(uint32_t rgba)
    {
        m_red = (rgba >> 24) & 0xFF;
        m_green = (rgba >> 16) & 0xFF;
        m_blue = (rgba >> 8) & 0xFF;
        m_alpha = rgba & 0xFF;
    }
};

/**
 * @brief Screen
 *
 * screenBuff is array that contains pixels as RGBA: 0xRRGGBBAA format
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
        updateScreen();
        return screenBuff;
    }

    void setSize(int h, int w);

    void updateScreen()
    {
        int pixel_ = 0;
        for (int i = 0; i < m_windowHeight; i++)
        {

            for (int j = 0; j < m_windowWidth; j++)
            {
                float color = i + j / m_windowWidth * m_windowHeight;

                Pixel pixel(static_cast<uint8_t>(color), static_cast<uint8_t>(color), static_cast<uint8_t>(color), static_cast<uint8_t>(255));
                screenBuff[pixel_] = pixel.serialize();
                pixel_++;
            }
        }
    }
};