#pragma once

#include <unordered_map>
#include <iostream>

#include "scene.hpp"

namespace sl
{
    struct ScenesManager
    {
    public:
        ScenesManager();
        std::unordered_map<std::string, Scene*> scenes;
        Scene* persistentScene;

        static Scene* AddScene(const std::string& sceneName);
        static void RemoveScene(const std::string& sceneName);
        static Scene* GetPersistentScene();
        static void LoadScene(const std::string& sceneName);
        static void LoadScene(const std::string& sceneName, bool additive);
        static void UnloadScene(const std::string& sceneName);
        static void UnloadAllScenes();
        static void UpdateRelevantScenes(const float deltaTime);
        static Scene* GetScene(const std::string& sceneName);
        static std::string& GetPreviousScene();
        static std::string& GetCurrentScene();
        void OnDestroy();
    private:
        static ScenesManager* instance;
        std::string current_scene;
        std::string previous_scene;
    };
}
