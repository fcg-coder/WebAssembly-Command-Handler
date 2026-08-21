#include "screen.hpp"
#include "shapes/shapes.hpp"
#include <algorithm>

namespace kernel
{
    Screen::Screen()
    {
        for (int y = 0; y < MAX_HEIGHT; ++y)
        {
            const int offset = y * MAX_WIDTH;

            for (int x = 0; x < MAX_WIDTH; ++x)
            {
                pixels[y][x] = Pixel(x, y, 0, 0, 0, 0);
                layoutIndixes[y][x] = -1;
                screenBuff[offset + x] = pixels[y][x].serialize();
            }
        }
        initializeScene();
    }

    uint32_t* Screen::getScreen()
    {
        clearScreen();
        render();

        return screenBuff;
    }

    void Screen::addShape(Pixel p, int layoutIndex)
    {
        if (p.x < 0 || p.x >= static_cast<int>(m_windowWidth) ||
            p.y < 0 || p.y >= static_cast<int>(m_windowHeight))
        {
            return;
        }

        if (layoutIndixes[p.y][p.x] == -1 ||
            layoutIndex <= layoutIndixes[p.y][p.x])
        {
            pixels[p.y][p.x] = p;
            layoutIndixes[p.y][p.x] = layoutIndex;
        }
    }

    void Screen::setSize(uint h, uint w)
    {
        m_windowHeight = std::min(h, static_cast<uint>(MAX_HEIGHT));
        m_windowWidth = std::min(w, static_cast<uint>(MAX_WIDTH));
    }

    void Screen::addObject(const char* key, ShapeBase* shape)
    {
        m_scene[key] = shape;
    }

    void Screen::removeObject(const char* key)
    {
        m_scene.erase(key);
    }

    std::pair<int, int> Screen::getSize()
    {
        return {
            static_cast<int>(m_windowWidth),
            static_cast<int>(m_windowHeight)};
    }

    void Screen::initializeScene()
    {

        addObject("cube", new Cube());
        // addObject("Gradient", new Gradient());
    }

    void Screen::renderObjects()
    {
        for (auto& [key, shape] : m_scene)
        {
            if (shape && shape->mode == ShapeMode::ON)
            {
                shape->render();
            }
        }
    }

    void Screen::render()
    {
        renderObjects();

        const int width = std::min(
            static_cast<int>(m_windowWidth),
            static_cast<int>(MAX_WIDTH));

        const int height = std::min(
            static_cast<int>(m_windowHeight),
            static_cast<int>(MAX_HEIGHT));

        const int bufferSize = std::min(
            width * height,
            static_cast<int>(MAX_SIZE));

        std::fill_n(
            screenBuff,
            bufferSize,
            0x00000000);

        for (int y = 0; y < height; ++y)
        {
            const int offset = y * width;

            for (int x = 0; x < width; ++x)
            {
                const int index = offset + x;

                if (index >= bufferSize)
                    break;

                screenBuff[index] = pixels[y][x].serialize();
            }
        }
    }

    void Screen::clearScreen()
    {
        const int width = std::min(
            static_cast<int>(m_windowWidth),
            static_cast<int>(MAX_WIDTH));

        const int height = std::min(
            static_cast<int>(m_windowHeight),
            static_cast<int>(MAX_HEIGHT));

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                pixels[y][x] = Pixel(x, y, 0, 0, 0, 0);
                layoutIndixes[y][x] = -1;
            }
        }
    }
} // namespace kernel