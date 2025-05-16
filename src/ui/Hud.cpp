#include "Hud.hpp"


static sf::Font& hudFont()
{
    static sf::Font font;
    static bool ok = font.openFromFile(Assets::FontSF);
    if (!ok) throw std::runtime_error("missing font");
    return font;
}

Hud::Hud(int& lives, int& score)
: m_lives (lives)
, m_score (score)
, m_txtLives (hudFont(), "Lives: 0", 24)   
, m_txtScore(hudFont(), "Score: 0", 24)
{
    m_txtLives .setFillColor(sf::Color::Red);
    m_txtScore .setFillColor(sf::Color::Yellow);

    m_txtLives .setPosition(sf::Vector2f{10.f, 10.f});
    m_txtScore .setPosition(sf::Vector2f{10.f, 40.f});
}

void Hud::update()
{
    m_txtLives .setString("Lives: " + std::to_string(m_lives));
    m_txtScore .setString("Score: " + std::to_string(m_score));
}

void Hud::draw(sf::RenderWindow& w) const
{
    w.draw(m_txtLives);
    w.draw(m_txtScore);
}
