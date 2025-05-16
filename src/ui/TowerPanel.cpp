#include "TowerPanel.hpp"
#include <stdexcept>
#include <iostream>

TowerPanel::TowerPanel(unsigned w, unsigned h, int& pts)
: m_points(pts)
{
    m_bg.setSize(sf::Vector2f{float(w), float(h)});
    m_bg.setFillColor(sf::Color(20,20,20,200));
    m_bg.setPosition(sf::Vector2f{1080.f - w, 0.f});

    if (!m_font.openFromFile("assets/fonts/OpenSans.ttf"))
        throw std::runtime_error("Missing font assets/fonts/OpenSans.ttf");

    const std::array<std::string,3> files = {
        "assets/sprites/normal.png",
        "assets/sprites/ice.png",
        "assets/sprites/bomb.png"
    };
    const int costs[3] = {10, 15, 30};
    const float iconSize = 40.f;

    m_icons.reserve(3);
    m_prices.reserve(3);

    for (int i=0;i<3;++i)
    {
        if (!m_texColor[i].loadFromFile(files[i]))
            throw std::runtime_error("Cannot load " + files[i]);
        m_texGray[i] = m_texColor[i];              

        sf::Sprite icon(m_texColor[i]);
        float sx = iconSize / m_texColor[i].getSize().x;
        float sy = iconSize / m_texColor[i].getSize().y;
        icon.setScale(sf::Vector2f{sx, sy});
        icon.setPosition(sf::Vector2f{
            1080.f - w/2.f - iconSize/2.f,
            60.f + i * 80.f });
        m_icons.push_back(icon);

        sf::Text lbl(m_font, std::to_string(costs[i]), 18);
        lbl.setOrigin(sf::Vector2f{ lbl.getLocalBounds().size.x / 2.f, 0.f });
        lbl.setFillColor(sf::Color::White);
        lbl.setPosition(icon.getPosition() + sf::Vector2f{0.f, 45.f});
        m_prices.push_back(lbl);
    }
}

void TowerPanel::update()
{
    const int costs[3] = {10, 15, 30};
    for (int i=0;i<3;++i)
    {
        bool afford = (m_points >= costs[i]);
        m_icons [i].setTexture( afford ? m_texColor[i] : m_texGray[i] );
        m_prices[i].setFillColor(
            afford ? sf::Color::White : sf::Color(140,140,140) );
    }
}

void TowerPanel::draw(sf::RenderWindow& win) const
{
    win.draw(m_bg);
    for (std::size_t i=0;i<m_icons.size(); ++i) {
        win.draw(m_icons[i]);
        win.draw(m_prices[i]);
    }
}

TowerType TowerPanel::handleClick(sf::Vector2i mouse) const
{
    for (int i=0;i<(int)m_icons.size(); ++i)
    {
        if (m_icons[i].getGlobalBounds()
              .contains(sf::Vector2f{float(mouse.x), float(mouse.y)}))
            return static_cast<TowerType>(i);   
    }
    return TowerType::None;
}
