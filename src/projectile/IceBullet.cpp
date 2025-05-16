#include "IceBullet.hpp"

IceBullet::IceBullet(sf::Vector2f start, Balloon* t)
: m_pos(start), m_target(t)
{
    m_shape.setRadius(5.f);
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setOrigin(sf::Vector2f{5.f,5.f});
    m_shape.setPosition(m_pos);
}

bool IceBullet::update(
    float dt,
    std::vector<std::unique_ptr<Balloon>>& /*balloons*/,
    int& /*playerPoints*/,
    int& /*playerHealth*/)
{
    if (!m_target || m_target->reachedGoal()) return true;

    sf::Vector2f dir = m_target->getPosition() - m_pos;
    float len = std::hypot(dir.x, dir.y);
    dir /= len;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);

    if (len < m_shape.getRadius() + m_target->getRadius()) {
        m_target->freeze(2.5f);   // 2.5-s freeze
        return true;
    }
    return false;
}

void IceBullet::draw(sf::RenderTarget& rt, sf::RenderStates st) const {
    rt.draw(m_shape, st);
}
