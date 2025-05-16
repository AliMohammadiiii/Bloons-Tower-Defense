#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

enum class TileType { Start, Finish, Path, Buildable };

class Map {
public:
    explicit Map(const std::string& filename);

    const std::vector<std::string>& grid()  const { return m_grid; }
    const std::vector<sf::Vector2i>& path() const { return m_path; }
    sf::Vector2i start()  const { return m_start; }
    sf::Vector2i finish() const { return m_finish; }
    TileType at(int r, int c) const;

private:
    void parseFile(const std::string& filename);
    void tracePath();
    bool inBounds(int r, int c) const;

    std::vector<std::string>  m_grid;
    std::vector<sf::Vector2i> m_path;
    sf::Vector2i m_start{-1,-1}, m_finish{-1,-1};
};
