#include "IceBullet.hpp"

IceBullet::IceBullet(sf::Vector2f start, Balloon* t)
: m_pos(start), m_target(t)
{
    m_shape.setRadius(5.f);
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setOrigin(sf::Vector2f{5.f,5.f});
    m_shape.setPosition(m_pos);
}

std::pair<bool, std::vector<std::unique_ptr<Balloon>>> IceBullet::update(
    float dt,
    std::vector<std::unique_ptr<Balloon>>& /*balloons*/,
    int& /*playerPoints*/)
{
    if (!m_target || m_target->reachedGoal()) return std::make_pair(true, std::vector<std::unique_ptr<Balloon>>{});

    sf::Vector2f dir = m_target->getPosition() - m_pos;
    float len = std::hypot(dir.x, dir.y);
    dir /= len;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);

    if (len < m_shape.getRadius() + m_target->getRadius()) {
        m_target->freeze(2.5f);   
        return std::make_pair(true, std::vector<std::unique_ptr<Balloon>>{});
    }
    return std::make_pair(false, std::vector<std::unique_ptr<Balloon>>{});
}

void IceBullet::draw(sf::RenderTarget& rt, sf::RenderStates st) const {
    rt.draw(m_shape, st);
}
