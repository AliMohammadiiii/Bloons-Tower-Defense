#include "NormalBullet.hpp"

NormalBullet::NormalBullet(sf::Vector2f start, Balloon* target)
: m_pos(start)
, m_target(target)
{
    m_shape.setRadius(5.f);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(sf::Vector2f{5.f, 5.f});
    m_shape.setPosition(m_pos);
}

std::pair<bool, std::vector<std::unique_ptr<Balloon>>> NormalBullet::update(
    float dt,
    std::vector<std::unique_ptr<Balloon>>& /*balloons*/,
    int& playerPoints)
{
    if (!m_target || m_target->isDead() || m_target->reachedGoal())
        return std::make_pair(true, std::vector<std::unique_ptr<Balloon>>{});

    sf::Vector2f dir = m_target->getPosition() - m_pos;
    float dist = std::hypot(dir.x, dir.y);
    dir /= dist;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);

    if (dist < m_shape.getRadius() + m_target->getRadius()) {
        playerPoints += m_target->score();
        auto kids = m_target->pop();
        m_target->kill();
        m_target = nullptr;
        return {true, std::move(kids)};

    }

    return std::make_pair(false, std::vector<std::unique_ptr<Balloon>>{}); 
}

void NormalBullet::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_shape, st);
}
