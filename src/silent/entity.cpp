#include "entity.hpp"
#include "scene.hpp"
#include "time.hpp"

namespace sl
{

    Entity::Entity(Scene* _scene) : uuid(UUID()) {
        scene = _scene;
    }

    Ref<Entity> Entity::CreateChildEntity(const std::string& name) {
        Ref<Entity> new_child = scene->AddEntity(name);

        new_child->parent = this;
        childEntities.push_back(new_child);

        return new_child;
    }

    Ref<Entity> Entity::GetChild(const std::string& name) const {
        for (auto& it : childEntities) {
            if (it->name == name) {
                return it;
            }
        }

        return nullptr;
    }
    Ref<Entity> Entity::GetChild(const u64& uuid) const {
        for (auto& it : childEntities) {
            if (it->uuid == uuid) {
                return it;
            }
        }

        return nullptr;
    }

    void Entity::RemoveChild(const Ref<Entity> child) {
        childEntities.erase(std::find(childEntities.begin(), childEntities.end(), child));
    }
    void Entity::RemoveChild(const std::string& name) {
        Ref<Entity> entity = GetChild(name);

        if (entity == nullptr) { return; }

        RemoveChild(entity);
    }
    void Entity::RemoveChild(const u64& uuid) {
        Ref<Entity> entity = GetChild(uuid);

        if (entity == nullptr) { return; }

        RemoveChild(entity);
    }

    void Entity::Awake()
    {
        for (auto& pair : componentList)
        {
            if (pair.second != nullptr)
            {
                pair.second->Awake();
            }
        }
    }

    void Entity::Update(const float deltaTime)
    {
        for (auto& pair : componentList)
        {
            if (pair.second != nullptr)
            {
                pair.second->Update(deltaTime);
            }
        }
    }

    void Entity::OnDestroy()
    {
        for (auto& pair : componentList)
        {
            if (pair.second != nullptr)
            {
                pair.second->OnDestroy();
            }
        }

        for (int timerID : relatedTimersIDs)
        {
            Time::RemoveTimer(timerID);
        }

        scene = nullptr;
    }

    void Entity::Disable()
    {
        for (auto& pair : componentList)
        {
            if (pair.second != nullptr)
            {
                pair.second->Disable();
            }
        }

        for (auto& it : childEntities) {
            it->Disable();
        }

        Enabled = false;
    }

    void Entity::Enable()
    {
        for (auto& pair : componentList)
        {
            if (pair.second != nullptr)
            {
                pair.second->Enable();
            }
        }

        for (auto& it : childEntities) {
            it->Enable();
        }

        Enabled = true;
    }

    void Entity::StartTimer(const float durationInMS, std::function<void(void)> callback)
	{
		relatedTimersIDs.push_back(
            Time::AddTimer(durationInMS, callback)
        );
	}
}