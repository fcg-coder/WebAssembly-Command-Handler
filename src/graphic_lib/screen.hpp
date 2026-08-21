#pragma once

#include <cstdint>
#include <utility>
#include <unordered_map>

#include "shapes/shape_base.hpp"
#include "colors.hpp"
#include "constants.hpp"
#include "base.hpp"

namespace kernel
{
    class Screen : public ScreenBase<Screen>
    {
        friend class kernel::ScreenBase<Screen>;

    public:
        uint32_t* getScreen();

        void addShape(Pixel p, int layoutIndex);
        void setSize(uint h, uint w);

        void addObject(const char* key, ShapeBase* shape);
        void removeObject(const char* key);
        std::pair<int, int> getSize();

    private:
        Screen();

        void initializeScene();
        void renderObjects();
        void render();
        void clearScreen();

        uint32_t screenBuff[MAX_SIZE];
        Pixel pixels[MAX_HEIGHT][MAX_WIDTH];
        int layoutIndixes[MAX_HEIGHT][MAX_WIDTH];
        std::unordered_map<const char*, ShapeBase*> m_scene;

        uint m_windowHeight = MAX_HEIGHT;
        uint m_windowWidth = MAX_WIDTH;
    };
} // namespace kernel