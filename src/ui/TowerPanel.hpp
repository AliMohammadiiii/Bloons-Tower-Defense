#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../tower/Tower.hpp"   // for TowerType

class TowerPanel {
public:
    TowerPanel(unsigned panelWidth,
               unsigned panelHeight,
               int& playerPoints);

    void draw(sf::RenderWindow& window) const;
    TowerType handleClick(sf::Vector2i mouse) const;
    void update();  // gray-out icons based on points

private:
    sf::RectangleShape           m_bg;
    std::vector<sf::Sprite>      m_icons;
    std::array<sf::Texture, 3>   m_texColor, m_texGray;
    int&                         m_points;
};
