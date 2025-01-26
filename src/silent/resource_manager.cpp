// resource_manager.cpp

#include "resource_manager.hpp"

namespace sl
{

    std::unordered_map<std::string, Ref<Resource>> ResourceManager::resources = std::unordered_map<std::string, Ref<Resource>>();

    void ResourceManager::ClearResources()
    {
        resources.clear();
    }

    const Ref<Resource> ResourceManager::Get(const std::string& path, ResourceType type)
    {
        if (resources.count(path) > 0) {
            return resources[path];
        }

        Ref<Resource> added = CreateRef<Resource>();
        added->type = type;
        switch (type) {

            case ResourceType::Texture:
                added->texture.loadFromFile(path);
                break;

            case ResourceType::Font:
                added->font.loadFromFile(path);
                break;

            default:
                break;
        }

        resources[path] = added;
        return added;
    }
}

