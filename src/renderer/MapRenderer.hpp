#pragma once
#include <SFML/Graphics.hpp>
#include "../map/Map.hpp"
#include "../constants.hpp"
#include <stdexcept>
#include <algorithm>   

class MapRenderer {
public:
    MapRenderer(const Map&             map,
                const std::string&     grassTex,
                const std::string&     roadTex,
                sf::Vector2u           windowSize,
                unsigned               panelWidth = Globals::panel_size);  

    void draw(sf::RenderWindow& win) const;

    float leftPad() const { return m_padX; }
    float topPad()  const { return m_padY; }

private:
    const Map& m_map;
    sf::Texture m_texGrass, m_texRoad;

    float m_padX{0.f}, m_padY{0.f};     
};
