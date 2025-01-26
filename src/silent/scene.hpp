// entity.hpp

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
//#include <unordered_map>


#include "common.hpp"
#include "Component.hpp"
#include "vector.hpp"
#include "resource_manager.hpp"
#include "transform.hpp"
#include "shape_type.hpp"
#include "multicast_delegate.hpp"
#include "LightSource.hpp"


namespace sl
{
    struct Scene
    {
    public:
        Scene();
        Scene(bool activeOnStartup);
        ~Scene();

        void Awake();
        void Update(const float deltaTime);
        void HandleCollisions();
        void OnDestroy();

        Ref<Entity> AddEntity(const std::string& name = "");
        Ref<Entity> AddExistingEntity(Ref<Entity> entity);

        Ref<Entity> GetEntity(const u64& uuid);
        Ref<Entity> GetEntity(const std::string& name);

        void RemoveEntity(Ref<Entity> entity);
        void RemoveEntity(const u64& uuid);
        void RemoveEntity(const std::string& name);
        void DiscardEntities();
        void DiscardEntity(Ref<Entity> entity);

        void AddLightSource(Ref<LightSource> newLightSource);
        void AddOnAwake(std::function<void(void)> newOnAwake);

        bool Active = false;
    private:
        friend class Renderer;

        std::vector<Ref<Entity>> entities;
        std::vector<Ref<Entity>> entitiesToDiscard; // discard after update loop to avoid messing with entitie list size during iteration and risking a throw
        std::vector<Ref<LightSource>> lightSources;
        MulticastDelegate<> onAwake = MulticastDelegate<>(true);
    };

    void InitLaunchScene(Scene* scene);
    void InitHomeScene(Scene* scene);
    void InitPauseScene(Scene* scene);
    void InitLvl1Scene(Scene* scene);
    void InitLvl2Scene(Scene* scene);
    void InitWinScene(Scene* scene);
    void InitGameOverScene(Scene* scene);
}
