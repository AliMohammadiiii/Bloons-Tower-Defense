// src/renderer/MapRenderer.cpp
#include "MapRenderer.hpp"
#include <stdexcept>
#include <algorithm>      

MapRenderer::MapRenderer(const Map&         map,
                         const std::string& grass,
                         const std::string& road,
                         sf::Vector2u       win,
                         unsigned           panelW)
: m_map(map)
{
    if (!m_texGrass.loadFromFile(grass) || !m_texRoad.loadFromFile(road))
        throw std::runtime_error("failed to load tile textures");

    const unsigned rows = static_cast<unsigned>(m_map.grid().size());
    const unsigned cols = static_cast<unsigned>(m_map.grid()[0].size());

    float usableW = static_cast<float>(win.x - panelW);
    float tile    = std::min(usableW / cols, static_cast<float>(win.y) / rows);

    m_padX = (usableW - tile * cols) / 2.f;
    m_padY = (static_cast<float>(win.y) - tile * rows) / 2.f;
    Globals::TILE = tile;
}

void MapRenderer::draw(sf::RenderWindow& win) const
{
    sf::RectangleShape square(sf::Vector2f{Globals::TILE, Globals::TILE});

    for (unsigned r = 0; r < m_map.grid().size(); ++r)
        for (unsigned c = 0; c < m_map.grid()[0].size(); ++c)
        {
            square.setPosition(sf::Vector2f{
                m_padX + c * Globals::TILE,
                m_padY + r * Globals::TILE });

            switch (m_map.at(r,c))
            {
              case TileType::Path:
              case TileType::Start:
              case TileType::Finish:
                  square.setTexture(&m_texRoad);  break;
              case TileType::Buildable:
                  square.setTexture(&m_texGrass); break;
            }
            win.draw(square);
        }
}
