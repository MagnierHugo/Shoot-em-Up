#include <iostream>
#include <fstream>

#include "json.hpp"

#include "tilemap.hpp"
#include "tile.hpp"
namespace sl
{
    bool TileMap::Load(const std::string &path)
    {
        nlohmann::json mapData;
        std::ifstream inputFile(path);

        if (!inputFile.is_open()) {
            std::cerr << "Failed to open file at: " << path << std::endl;
            return false;
        }

        inputFile >> mapData;

        std::cout << "Read: " << mapData.dump(4) << std::endl;

        int mapWidth = mapData["width"];
        int mapHeight = mapData["height"];

        Clear();

        for (int y = 0; y < mapHeight; y++)
        {
            map.push_back({});
            for (int x = 0; x < mapWidth; x++)
            {
                map[y].push_back(Tile::FromData(mapData["tiles"][y][x]));
            }
        }

        return true;
    }

    void TileMap::Save(const std::string &path)
    {
        nlohmann::json data;

        data["height"] = map.size();
        data["width"] = map[0].size();

        // data["tiles"] = {};
        data["tiles"] = nlohmann::json::array();
        for (int y = 0; y < map.size(); y++)
        {
            data["tiles"].push_back(nlohmann::json::array());
            std::cout << "added row" << std::endl;
            for (int x = 0; x < map[0].size(); x++) // replace with y ig want dynamic
            {
                std::cout << "Wrote: " << data.dump(4) << std::endl;
                data["tiles"][y].push_back(map[y][x] == nullptr ? nullptr : map[y][x]->GetData());
                std::cout << "actually added tile" << std::endl;
            }
        }

        std::cout << "Wrote: " << data.dump(4) << std::endl;

        std::ofstream o(path);
        o << std::setw(4) << data << std::endl;
    }

    void TileMap::AddTile(Tile* newTile, int x, int y)
    {
        while ((int)map.size() - 1 < y)
        {   
            map.push_back(std::vector<Tile*>());
        }

        while ((int)map[y].size() -1 < x)
        {
            map[y].push_back(nullptr);
        }

        map[y][x] = newTile;
    }

    void TileMap::Clear()
    {
        for (int y = 0; y < map.size(); y++)
        {
            for (int x = 0; x < map[0].size(); x++) // replace with y if want dynamic
            {
                delete map[y][x];
                map[y][x] = nullptr;
            }
        }

        map.clear();
    }
}