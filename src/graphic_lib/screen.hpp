#pragma once
#include <cstdint>
#include <cstdio>
#include "../main.hpp"
#include "shapes/shape_base.hpp"
#include "shapes/shapes.hpp"
#include <random>
#include <utility>

#ifdef SIMD
#    include <wasm_simd128.h>

#endif

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

/**
 * @brief Pixels class
 */
class Pixel
{
public:
    int x; // abscissa
    int y; // ordinate
    Color m_color;

    Pixel(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : m_color(red, green, blue, alpha), x(0), y(0) {}

    Pixel(int x, int y, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
        : x(x), y(y), m_color(red, green, blue, alpha) {}

    explicit Pixel(const Color& color, int x = 0, int y = 0)
        : m_color(color), x(x), y(y) {}

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

/**
 * @brief Screen class with 2D array implementation
 */
class Screen
{
private:
    void initializeScene();

    Screen()
    {
        for (int y = 0; y < MAX_HEIGHT; ++y)
        {
            for (int x = 0; x < MAX_WIDTH; ++x)
            {
                pixels[y][x] = Pixel(x, y, 0, 0, 0, 0);
                layoutIndices[y][x] = -1;
                const int index = y * MAX_WIDTH + x;
                if (index < MAX_SIZE)
                {
                    screenBuff[index] = pixels[y][x].serialize();
                }
            }
        }
        initializeScene();
    }

    // Cube cube;
    static Screen* m_instance;
    std::map<std::string, ShapeBase*> m_scene; // map of objects with string keys
    Pixel pixels[MAX_HEIGHT][MAX_WIDTH];       // PIXELS ON SCREEN
    int layoutIndices[MAX_HEIGHT][MAX_WIDTH];

    static int m_windowHeight;
    static int m_windowWidth;

    void render()
    {
        const int current_width = m_windowWidth;
        const int current_height = m_windowHeight;
        const int buffer_size = current_width * current_height;

        std::fill_n(screenBuff, (buffer_size <= MAX_SIZE) ? buffer_size : MAX_SIZE, 0x00000000);

// https://emscripten.org/docs/porting/simd.html
#if defined(SIMD) && defined(__wasm_simd128__)
#    include <wasm_simd128.h>

        for (int y = 0; y < current_height; ++y)
        {
            int x = 0;

            for (; x <= current_width - 4; x += 4)
            {
                const int index = y * current_width + x;

                v128_t pixel_data = wasm_u32x4_make(
                    pixels[y][x].serialize(),
                    pixels[y][x + 1].serialize(),
                    pixels[y][x + 2].serialize(),
                    pixels[y][x + 3].serialize());

                wasm_v128_store(&screenBuff[index], pixel_data);
            }

            for (; x < current_width; ++x)
            {
                const int index = y * current_width + x;
                screenBuff[index] = pixels[y][x].serialize();
            }
        }
#else
        for (int y = 0; y < current_height; ++y)
        {
            for (int x = 0; x < current_width; ++x)
            {
                const int index = y * current_width + x;
                if (index < MAX_SIZE)
                {
                    screenBuff[index] = pixels[y][x].serialize();
                }
            }
        }
#endif
    }

    void clearScreen()
    {
        const int current_width = m_windowWidth;
        const int current_height = m_windowHeight;

        for (int y = 0; y < current_height; ++y)
        {
            for (int x = 0; x < current_width; ++x)
            {
                pixels[y][x] = Pixel(x, y, 0, 0, 0, 0);
                layoutIndices[y][x] = -1;
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
        clearScreen();
        if (m_windowHeight > MAX_HEIGHT || m_windowWidth > MAX_WIDTH)
        {
            return nullptr;
        }
        for (auto& [key, shape] : m_scene)
        {
            if (shape && shape->mode == ShapeMode::ON)
            {
                shape->render(); // render the object
            }
        }

        render(); // render all screen
        return screenBuff;
    }

    /**
     * @brief Add a PIXELS to the pixels array. Usually used for lines and points.
     */
    void addShape(Pixel p, int layoutIndex)
    {
        if (p.x >= 0 && p.x < m_windowWidth && p.y >= 0 && p.y < m_windowHeight)
        {

            if (layoutIndex <= layoutIndices[p.y][p.x] || layoutIndices[p.y][p.x] == -1)
            {
                pixels[p.y][p.x] = p;
                layoutIndices[p.y][p.x] = layoutIndex;
            }
        }
    }

    void setSize(int h, int w);

    /**
     * @brief Add an object to the scene with a string key.
     *
     * Used to switch on/off the object's rendering in the future.
     */
    void addObject(const std::string& key, ShapeBase* shape);

    /**
     * @brief Remove an object from the scene by its string key.
     */
    void removeObject(const std::string& key)
    {
        m_scene.erase(key);
    }

    /**
     * @brief Get an object from the scene by its string key.
     *
     * @return Pointer to the ShapeBase object, or nullptr if not found.
     */
    ShapeBase* getObject(const std::string& key) const
    {
        auto it = m_scene.find(key);
        return (it != m_scene.end()) ? it->second : nullptr;
    }

    std::pair<int, int> getSize()
    {
        return {m_windowWidth, m_windowHeight};
    }
};
