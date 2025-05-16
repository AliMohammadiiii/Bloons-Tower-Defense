#include "MapRenderer.hpp"
#include "../constants.hpp"
#include <stdexcept>
#include <algorithm>

MapRenderer::MapRenderer(const Map& m,const std::string& grass,
                         const std::string& road, sf::Vector2u win)
: m_map(m)
{
    if(!m_texGrass.loadFromFile(grass))
        throw std::runtime_error("load "+grass);
    if(!m_texRoad.loadFromFile(road ))
        throw std::runtime_error("load "+road );

    const auto& g=m_map.grid();
    int rows=g.size(), cols=g[0].size();
    float tile = std::min(win.x / cols, win.y / rows);

    m_padX = (win.x - tile * cols) / 2.f;   // <-- store
    m_padY = (win.y - tile * rows) / 2.f;   // <-- store
    Globals::TILE = tile;   

    float padX=(win.x-tile*cols)/2.f;
    float padY=(win.y-tile*rows)/2.f;

    m_tiles.reserve(rows*cols);

    for(int r=0;r<rows;++r)
        for(int c=0;c<cols;++c){
            const sf::Texture& tex =
               (g[r][c]=='-'? m_texGrass : m_texRoad);
            sf::Sprite s(tex);
            s.setScale(sf::Vector2f{ tile/tex.getSize().x,
                                     tile/tex.getSize().y });
            s.setPosition(sf::Vector2f{ padX+c*tile, padY+r*tile });
            m_tiles.push_back(s);
        }
}

void MapRenderer::draw(sf::RenderWindow& w) const {
    for(const auto& s: m_tiles) w.draw(s);
}
