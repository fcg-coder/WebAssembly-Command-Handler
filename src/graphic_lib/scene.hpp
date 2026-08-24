#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "shapes/shape_base.hpp"

namespace scene
{
    /*
     * @todo наследники могут переопределять update
     * объекты базового класса не обновляются
     */
    class Scene final
    {
    public:
        Scene() = default;
        ~Scene() = default;

        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) noexcept = default;
        Scene& operator=(Scene&&) noexcept = default;

        void initialize();
        void update();
        void render();

        void addObject(const std::string& key,std::unique_ptr<ShapeBase> shape);
        void removeObject(const std::string& key);

    private:
        void renderObjects();
        std::unordered_map<std::string,std::unique_ptr<ShapeBase>> m_objects;
    };

    class SceneRegister final
    {
    public:
        SceneRegister() = default;
        ~SceneRegister() = default;

        SceneRegister(const SceneRegister&) = delete;
        SceneRegister& operator=(const SceneRegister&) = delete;

        SceneRegister(SceneRegister&&) noexcept = default;
        SceneRegister& operator=(SceneRegister&&) noexcept = default;

        void add(const std::string& key,std::unique_ptr<Scene> scene);

        void remove(const std::string& key);

        Scene* get(const std::string& key);

        Scene* load(const std::string& key);

        Scene* current();

        const Scene* current() const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Scene> > m_scenes;
        Scene* m_currentScene = nullptr;
    };
}