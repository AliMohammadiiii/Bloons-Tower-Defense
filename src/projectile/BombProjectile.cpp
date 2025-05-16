#include "BombProjectile.hpp"

BombProjectile::BombProjectile(
    sf::Vector2f start, Balloon* t, float r)
: m_pos(start), m_target(t), m_radius(r)
{
    m_shape.setRadius(6.f);
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setOrigin(sf::Vector2f{6.f,6.f});
    m_shape.setPosition(m_pos);
}

bool BombProjectile::update(
    float dt,
    std::vector<std::unique_ptr<Balloon>>& balloons,
    int& playerPoints,
    int& /*playerHealth*/)
{
    if (!m_target || m_target->reachedGoal()) return true;

    sf::Vector2f dir = m_target->getPosition() - m_pos;
    float len = std::hypot(dir.x, dir.y);
    dir /= len;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);

    if (len < m_shape.getRadius() + m_target->getRadius()) {
        // explode: kill every balloon within radius
        for (auto& b : balloons) {
            float d = std::hypot(b->getPosition().x - m_pos.x,
                                 b->getPosition().y - m_pos.y);
            if (d <= m_radius) {
                playerPoints += b->score();
                auto kids = b->pop();
                b->kill();
                for (auto& k : kids) balloons.push_back(std::move(k));
            }
        }
        return true;
    }
    return false;
}

void BombProjectile::draw(sf::RenderTarget& rt, sf::RenderStates st) const {
    rt.draw(m_shape, st);
}
