
#include "sprite_manager.hpp"



namespace sl 
{

    void SpriteManager::LoadTexture(sf::Texture texture, std::string file_path, sf::RenderWindow& window)
    {
        if(!texture.loadFromFile(file_path))
        {
            std::cerr<<"Unable to load texture\n";
        }
    }

    void SpriteManager::SpriteInit()
    {
        
    }
      
}
