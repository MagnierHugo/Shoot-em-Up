#include "entity.hpp"

template<typename TComponent> sl::Ref<TComponent> sl::Entity::AddComponent()
{
    Ref<TComponent> componentPtr = CreateRef<TComponent>();
    componentList[typeid(TComponent).name()] = componentPtr;

    std::static_pointer_cast<Component>(componentPtr)->SetOwner(this);

    return componentPtr;
}

template<typename TComponent> bool sl::Entity::TryGetComponent(sl::Ref<TComponent>& returnPtr)
{
    auto it = componentList.find(typeid(TComponent).name());
    if (it == componentList.end())
    {
        returnPtr = nullptr;
        return false;
    }
    else
    {
        returnPtr = std::static_pointer_cast<TComponent>(it->second);
        return true;
    }
}

template<typename TComponent> sl::Ref<TComponent> sl::Entity::GetComponent()
{
    return std::static_pointer_cast<TComponent>(componentList.find(typeid(TComponent).name())->second);
}