#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include <string>

#include <vector>
namespace sl 
{
    class SpriteManager
    {
        public:
        SpriteManager();
        ~SpriteManager() = default;

        void LoadTexture(sf::Texture texture, std::string file_path, sf::RenderWindow& window);
        void RenderTexture(sf::RenderWindow& window);
        void SpriteInit();

        private:

        std::vector<Sprite> sprite_array;
        std::vector<sf::Texture> texture_array;
    };


}


class Sprite
{
    public:

    Sprite(sf::Texture texture, sf::Vector2f size(int width, int height), sf::Vector2f position(int x, int y) )
    {
        texture = texture;
        size = size;
        position = position;
    };

    private:
    //To create different type of sprites

    enum class staticSpriteType
    {
        
    };

    enum class dynamicSpriteType
    {

    };

    enum class backgroundSpriteType
    {

    };

    enum class UISpriteType
    {

    };

    sf::Texture texture;
    sf::Vector2f size(int width, int height);
    sf::Vector2f position(int x, int y);

};
