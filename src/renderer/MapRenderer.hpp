#pragma once
#include <SFML/Graphics.hpp>
#include "../map/Map.hpp"

class MapRenderer {
public:
    MapRenderer(const Map&, const std::string& grassImg,
                const std::string& roadImg, sf::Vector2u windowSize);

    void draw(sf::RenderWindow&) const;
    float leftPad()  const { return m_padX; }
    float topPad()   const { return m_padY; }

private:
    const Map& m_map;
    sf::Texture m_texGrass, m_texRoad;
    std::vector<sf::Sprite> m_tiles;
    float m_padX, m_padY;
};
