#pragma once

#include <vector>
#include <string>


#include "tile.hpp"
namespace sl
{
    class TileMap
    {
        public:
            int tileWidth;
            int tileHeight;
            
            bool Load(const std::string& path);
            void Save(const std::string& path);
            void Clear();
            void AddTile(Tile* newTile, int x, int y);
        private:
            std::vector<std::vector<Tile*>> map;    
    };
}