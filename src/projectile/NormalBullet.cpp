#include "NormalBullet.hpp"

NormalBullet::NormalBullet(sf::Vector2f start, Balloon* t)
: m_pos(start), m_target(t)
{
    m_shape.setRadius(5.f);
    m_shape.setFillColor(sf::Color::Black);
    m_shape.setOrigin(sf::Vector2f{5.f, 5.f});
    m_shape.setPosition(m_pos);
}

bool NormalBullet::update(float dt,
    std::vector<std::unique_ptr<Balloon>>& balloons,
    int& playerPoints, int& playerHealth)
{
    if (!m_target || m_target->reachedGoal()) return true; // remove

    // move towards target
    auto dir = m_target->getPosition() - m_pos;
    float len = std::hypot(dir.x, dir.y);
    dir /= len;
    m_pos += dir * m_speed * dt;
    m_shape.setPosition(m_pos);


    float dist = std::hypot(m_pos.x - m_target->getPosition().x,
                            m_pos.y - m_target->getPosition().y);
    if (dist < m_shape.getRadius() + m_target->getRadius())
    {
        // award points
        playerPoints += m_target->score();

        // spawn any children
        auto kids = m_target->pop();
        for (auto& k : kids)
            balloons.push_back(std::move(k));

        // mark this balloon dead so Game::update will erase it
        m_target->kill();

        return true;  // remove this bullet
    }
    return false; // keep bullet
}

void NormalBullet::draw(sf::RenderTarget& rt, sf::RenderStates) const {
    rt.draw(m_shape, sf::RenderStates::Default);
}
