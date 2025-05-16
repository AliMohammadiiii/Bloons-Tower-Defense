#include "TowerPanel.hpp"
#include <stdexcept>

TowerPanel::TowerPanel(unsigned w, unsigned h, int& pts)
: m_points(pts)
{
    // Background panel on right edge
    m_bg.setSize(sf::Vector2f{float(w), float(h)});
    m_bg.setFillColor(sf::Color(20, 20, 20, 200));
    m_bg.setPosition(sf::Vector2f{1080.f - w, 0.f});

    // Paths to your 3 tower icons
    const std::array<std::string,3> files = {
        "assets/sprites/normal.png",
        "assets/sprites/ice.png",
        "assets/sprites/bomb.png"
    };

    const float iconSize = 40.f;        // desired drawn size
    m_icons.reserve(3);

    for (int i = 0; i < 3; ++i) {
        // Load color texture
        if (!m_texColor[i].loadFromFile(files[i]))
            throw std::runtime_error("Could not load " + files[i]);
        // For now, gray = color copy (you can desaturate later)
        m_texGray[i] = m_texColor[i];

        // Build a sprite and scale it
        sf::Sprite sprite(m_texColor[i]);
        float sx = iconSize / m_texColor[i].getSize().x;
        float sy = iconSize / m_texColor[i].getSize().y;
        sprite.setScale(sf::Vector2f{sx, sy});  // SFML3 requires single Vector2f
        sprite.setPosition(sf::Vector2f{
            1080.f - w/2.f - iconSize/2.f,   // center in panel
            60.f + i * 80.f                  // vertical spacing
        });

        m_icons.push_back(sprite);
    }
}

void TowerPanel::update()
{
    // Costs match Normal=10, Ice=15, Bomb=30
    const int costs[3] = { 10, 15, 30 };
    for (int i = 0; i < 3; ++i) {
        bool affordable = (m_points >= costs[i]);
        m_icons[i].setTexture(affordable ? m_texColor[i] : m_texGray[i]);
    }
}

void TowerPanel::draw(sf::RenderWindow& window) const
{
    window.draw(m_bg);
    for (const auto& icon : m_icons)
        window.draw(icon);
}

TowerType TowerPanel::handleClick(sf::Vector2i mouse) const
{
    // Check each icon’s bounds
    for (int i = 0; i < (int)m_icons.size(); ++i) {
        auto bounds = m_icons[i].getGlobalBounds();
        // SFML3: contains takes a single Vector2f
        if (bounds.contains(sf::Vector2f{
                float(mouse.x), float(mouse.y)
            }))
        {
            return static_cast<TowerType>(i);  // 0=Normal,1=Ice,2=Bomb
        }
    }
    return TowerType::None;
}
