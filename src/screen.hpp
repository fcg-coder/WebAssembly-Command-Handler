#pragma once
#include <cstdint>

#define MAX_WIDTH 1920
#define MAX_HEIGH 1080

#define SIZE (MAX_WIDTH * MAX_WIDTH)

/**
 * @brief Screen
 *
 * screenBuff is array that contains pixels as RGBA: 0xRRGGBBAA format
 *
 */
class Screen
{
private:
    Screen() = default;
    static class Screen* m_instance;

public:
    uint32_t screenBuff[SIZE];
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
    void updateScreen()
    {
        for (int i = 0; i < SIZE; i++)
        {
            screenBuff[i] = 0xFF0000FF; // Красный (RGBA: 0xRRGGBBAA)
        }
    }
};