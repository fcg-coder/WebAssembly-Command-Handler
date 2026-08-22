#pragma once

#include <cstdint>
#include <utility>

#include "shapes/shape_base.hpp"
#include "colors.hpp"
#include "constants.hpp"
#include "base.hpp"
#include "scene.hpp"

namespace kernel
{
    class Screen : public ScreenBase<Screen>
    {
        friend class kernel::ScreenBase<Screen>;
    public:
        uint32_t* getScreen();

        void addShape(Pixel p, int layoutIndex);

        void setSize(uint h,uint ws);

        std::pair<int, int> getSize();

        bool loadScene(const std::string& key) { return m_scenes.load(key) != nullptr; }
    private:
        Screen();

        static inline bool isInited = false;

        void initializeScene();

        void render();
        void renderObjects();
        void clearScreen();

        uint32_t screenBuff[MAX_SIZE];

        Pixel pixels[MAX_HEIGHT][MAX_WIDTH];

        int layoutIndixes[MAX_HEIGHT][MAX_WIDTH];

        scene::SceneRegister m_scenes;

        uint m_windowHeight = MAX_HEIGHT;
        uint m_windowWidth = MAX_WIDTH;
    };
}