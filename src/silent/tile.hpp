#pragma once

#include "json.hpp"

class Tile
{
    public:
        Tile(int x, int y)
        {
            X = x;
            Y = y;
        };
        int X;
        int Y;
        static Tile* FromData(nlohmann::json& data)
        {
            Tile* newTile = new Tile(data["x"], data["y"]);

            // actually setup the tile as needed

            return newTile;
        }

        nlohmann::json GetData()
        {
            nlohmann::json data;

            // actually pack the data inside
            data["x"] = X;
            data["y"] = Y;

            return data;
        }
    private:
};