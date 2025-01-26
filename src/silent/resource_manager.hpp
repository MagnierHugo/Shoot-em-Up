// resource_manager.hpp

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

#include "common.hpp"

namespace sl
{
    enum class ResourceType
    {
        Texture,
        Font
    };

    struct Resource
    {
        ResourceType type;

        sf::Texture texture;
        sf::Font font;
    };

    class ResourceManager
    {
    public:
        static void ClearResources();
        static const Ref<Resource> Get(const std::string& path, ResourceType type);
    private:
        static std::unordered_map<std::string, Ref<Resource>> resources;
        
    };
}
