#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include "../tower/Tower.hpp"  
#include "../constants.hpp"
#include <stdexcept>

class TowerPanel {
public:
    TowerPanel(unsigned panelWidth, unsigned panelHeight, int& playerPoints);

    void update();                     
    void draw(sf::RenderWindow&) const;
    TowerType handleClick(sf::Vector2i mouse) const;

private:
    sf::RectangleShape              m_bg;
    std::vector<sf::Sprite>         m_icons;
    std::vector<sf::Text>           m_prices;   
    std::array<sf::Texture, 3>      m_texColor, m_texGray;
    sf::Font                        m_font;    
    int&                            m_points;
};
