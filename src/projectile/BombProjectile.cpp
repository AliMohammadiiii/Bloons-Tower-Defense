#include "BombProjectile.hpp"
#include <iostream>
BombProjectile::BombProjectile(
    sf::Vector2f start, Balloon* target, float explosionRadius)
: m_pos(start)
, m_target(target)
, m_radius(explosionRadius)
{
    m_shape.setRadius(6.f);
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOrigin(sf::Vector2f{6.f, 6.f});
    m_shape.setPosition(m_pos);
}

std::pair<bool, std::vector<std::unique_ptr<Balloon>>> BombProjectile::update(
    float dt,
    std::vector<std::unique_ptr<Balloon>>& balloons,
    int& playerPoints)
{
    if (!m_target || m_target->isDead() || m_target->reachedGoal())
        return std::make_pair(true, std::vector<std::unique_ptr<Balloon>>{});


    sf::Vector2f dir = m_target->getPosition() - m_pos;
    float len = std::hypot(dir.x, dir.y);
    dir /= len;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);
    std::vector<std::unique_ptr<Balloon>> newKids;
    
    if (len < m_shape.getRadius() + m_target->getRadius()) {
        for (auto& b : balloons) {
            float d = std::hypot(b->getPosition().x - m_pos.x,
                                 b->getPosition().y - m_pos.y);
            if (d <= m_radius) {
                playerPoints += b->score();
                auto kids = b->pop();
                b->kill();
                for (auto& k : kids) newKids.push_back(std::move(k));
            }
        }
        return {true, std::move(newKids)};
    }
    return std::make_pair(false, std::vector<std::unique_ptr<Balloon>>{});

}

void BombProjectile::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_shape, st);
}
