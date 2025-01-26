#include <cassert>

#include "scenes_manager.hpp"
#include "input_manager.hpp"

namespace sl
{
    ScenesManager* ScenesManager::instance = nullptr;

    ScenesManager::ScenesManager()
    {
        instance = this;

        persistentScene = new Scene(true);
    }

    void ScenesManager::UnloadScene(const std::string& sceneName)
    {
        auto& it = instance->scenes.find(sceneName);
        assert(it != instance->scenes.end() && "Scene name not found");
        it->second->Active = false;
    }

    void ScenesManager::UnloadAllScenes()
    {
        for (auto& [_, scene] : instance->scenes)
        {
            scene->Active = false;
        }
    }

    void ScenesManager::LoadScene(const std::string& sceneName)
    {
        LoadScene(sceneName, false);
    }

    void ScenesManager::LoadScene(const std::string& sceneName, bool additive)
    {
        if (!additive)
        {
            UnloadAllScenes();
        }

        auto& it = instance->scenes.find(sceneName);
        assert(it != instance->scenes.end() && "Scene name not found");
        it->second->Active = true;

        instance->previous_scene = instance->current_scene;
        instance->current_scene = sceneName;

        InputManager::ResetInputState();
    }

    Scene* ScenesManager::AddScene(const std::string& sceneName)
    {
        assert(instance->scenes.find(sceneName) == instance->scenes.end() && "Scene name already exists");

        Scene* newScene = new Scene();
        instance->scenes[sceneName] = newScene;

        return newScene;
    }

    void ScenesManager::RemoveScene(const std::string& sceneName) {
        assert(instance->scenes.find(sceneName) != instance->scenes.end() && "Scene name doesn't exist");

        instance->scenes[sceneName]->OnDestroy();
        instance->scenes.erase(sceneName);
    }

    Scene* ScenesManager::GetPersistentScene()
    {
        return instance->persistentScene;
    }

    void ScenesManager::UpdateRelevantScenes(const float deltaTime)
    {
        instance->persistentScene->Update(deltaTime);

        for (auto& [_, scene] : instance->scenes)
        {
            if (!scene->Active) { continue; }

            scene->Update(deltaTime);
        }
    }

    Scene* ScenesManager::GetScene(const std::string& sceneName)
    {
        auto& it = instance->scenes.find(sceneName);
        assert(it != instance->scenes.end() && "Scene name not found");
        return it->second;
    }

    void ScenesManager::OnDestroy()
    {
        persistentScene->OnDestroy();
        delete persistentScene;

        for (auto& [_, scene] : scenes)
        {
            scene->OnDestroy();
            delete scene;
        }
    }

    std::string& ScenesManager::GetPreviousScene() {
        return instance->previous_scene;
    }

    std::string& ScenesManager::GetCurrentScene() {
        return instance->current_scene;
    }
}

