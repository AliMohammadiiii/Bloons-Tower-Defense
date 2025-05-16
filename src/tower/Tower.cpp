#include "Tower.hpp"

Tower::Tower(sf::Vector2f pos, float range, float cooldown, int cost)
: m_pos(pos)
, m_range(range)
, m_cooldown(cooldown)
, m_cost(cost)
{
    m_rangeCircle.setRadius(m_range);
    m_rangeCircle.setOrigin(sf::Vector2f{ m_range, m_range });
    m_rangeCircle.setPosition(m_pos);
    m_rangeCircle.setFillColor(sf::Color(0,0,0,0));
    m_rangeCircle.setOutlineColor(sf::Color(255,255,255,128));
    m_rangeCircle.setOutlineThickness(1.f);
}

bool Tower::canFire() const { return m_timer >= m_cooldown; }
void Tower::resetCooldown()  { m_timer = 0.f; }

bool Tower::isInRange(const sf::Vector2f& p) const {
    auto d = p - m_pos;
    return (d.x*d.x + d.y*d.y) <= (m_range * m_range);
}

void Tower::draw(sf::RenderTarget& rt, sf::RenderStates st) const {
    rt.draw(m_rangeCircle, st);
}

