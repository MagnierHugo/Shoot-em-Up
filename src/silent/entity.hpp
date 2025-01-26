#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>

#include "common.hpp"




namespace sl
{
    
    class Component;
    class Scene;
    class Transform;

    // NOTE(amelie): The megastruct. The enemy of the encapsulation man. There is no god anymore. We killed him
    struct Entity
    {
        // NOTE(amelie): Entity descriptors
        const u64 uuid; // NOTE(hugo): Made uuid const as it always should have been
        std::string name;

        Scene* scene = nullptr;
        Entity* parent = nullptr;
        std::vector<Ref<Entity>> childEntities;

        // NOTE(antoine): I wont even describe
        Ref<Transform> transform = nullptr;
        
        // NOTE(amelie): Colliders
        bool has_collider = false;
        bool is_sensor = false;
        std::function<void(Ref<Entity> sender, Ref<Entity> receiver)> collider_callback;

        Entity(Scene* _scene);

        Ref<Entity> CreateChildEntity(const std::string& name);

        Ref<Entity> GetChild(const std::string& name) const;
        Ref<Entity> GetChild(const u64& uuid) const;

        void RemoveChild(const std::string& name);
        void RemoveChild(const u64& uuid);
        void RemoveChild(const Ref<Entity> child);

        template<typename TComponent> Ref<TComponent> AddComponent();
        template<typename TComponent> bool TryGetComponent(Ref<TComponent>& returnPtr);
        template<typename TComponent> Ref<TComponent> GetComponent();

        void Awake();
        void Update(const float deltaTime);
        void OnDestroy();

        void Disable();
        void Enable();
        bool Enabled = true;
        
        void StartTimer(const float durationInMS, std::function<void(void)> callback);

    private:
        std::unordered_map<std::string, Ref<Component>> componentList;
        std::vector<int> relatedTimersIDs;

    };

#include "entity.inl"
}
