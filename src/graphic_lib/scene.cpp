#include "scene.hpp"

namespace scene
{
    void Scene::initialize()
    {
    }


    void Scene::update()
    {
    }


    void Scene::render()
    {
        renderObjects();
    }


    void Scene::addObject(
        const std::string& key,
        std::unique_ptr<ShapeBase> shape
    )
    {
        if (!shape)
            return;

        m_objects[key] = std::move(shape);
    }


    void Scene::removeObject(
        const std::string& key
    )
    {
        m_objects.erase(key);
    }


    void Scene::renderObjects()
    {
        for (auto& [key, shape] : m_objects)
        {
            if (!shape)
                continue;

            if (shape->mode != isVisible::ON)
                continue;

            shape->render();
        }
    }


    // =========================================================
    // SceneRegister
    // =========================================================

    void SceneRegister::add(
        const std::string& key,
        std::unique_ptr<Scene> scene
    )
    {
        if (!scene)
            return;

        m_scenes[key] = std::move(scene);
    }


    void SceneRegister::remove(
        const std::string& key
    )
    {
        auto it = m_scenes.find(key);

        if (it == m_scenes.end())
            return;

        if (m_currentScene == it->second.get())
            m_currentScene = nullptr;

        m_scenes.erase(it);
    }


    Scene* SceneRegister::get(
        const std::string& key
    )
    {
        auto it = m_scenes.find(key);

        if (it == m_scenes.end())
            return nullptr;

        return it->second.get();
    }


    Scene* SceneRegister::load(
        const std::string& key
    )
    {
        Scene* scene = get(key);

        if (!scene)
            return nullptr;

        m_currentScene = scene;

        m_currentScene->initialize();

        return m_currentScene;
    }


    Scene* SceneRegister::current()
    {
        return m_currentScene;
    }


    const Scene* SceneRegister::current() const
    {
        return m_currentScene;
    }
}