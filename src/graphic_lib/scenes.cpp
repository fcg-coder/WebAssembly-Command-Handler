#include "screen.hpp"
#include "shapes/shapes.hpp"

#include <memory>

namespace kernel
{
    void Screen::initializeScene()
    {
        auto pyramidScene = std::make_unique<scene::Scene>();
        auto cubeScene = std::make_unique<scene::Scene>();

        pyramidScene->addObject("Pyramid",std::make_unique<Pyramid>());

        cubeScene->addObject("Cube", std::make_unique<Cube>());

        m_scenes.add("pyramidScene",std::move(pyramidScene));

        m_scenes.add("cubeScene",std::move(cubeScene) );

        m_scenes.load("cubeScene");

        isInited = true;
    }
}