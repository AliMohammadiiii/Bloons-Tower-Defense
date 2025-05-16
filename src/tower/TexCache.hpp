#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

inline sf::Texture& getTowerTex(const std::string& path)
{
    static std::map<std::string, sf::Texture*> cache;

    if (auto it = cache.find(path); it != cache.end())
        return *it->second;

    auto tex = new sf::Texture;                   
    if (!tex->loadFromFile(path))
        throw std::runtime_error("Cannot load " + path);

    cache[path] = tex;
    return *tex;
}
