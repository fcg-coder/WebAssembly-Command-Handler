#include "screen.hpp"
#include "shapes/shapes.hpp"

#include <memory>

#include "../kernel/kernel.hpp"

namespace kernel
{
    void Screen::initializeScene()
    {
        auto singleObjectScene = [this](const std::string& sceneName, const std::string& objectName, auto object)
            {
                auto scene = std::make_unique<scene::Scene>();
                scene->addObject(objectName, std::move(object));
                m_scenes.add(sceneName,std::move(scene));
                Kernel::IOH()->output("Load scene %s", sceneName.c_str());
            };

        singleObjectScene("pyramid","Pyramid",std::make_unique<Pyramid>());

        singleObjectScene("cube","Cube",std::make_unique<Cube>());

        singleObjectScene("spline","Spline2D",std::make_unique<Spline2D>());

        singleObjectScene("sphere","RayTracedSphere",std::make_unique<RayTracedSphere>());

        m_scenes.load("spline");

        isInited = true;
    }
}
