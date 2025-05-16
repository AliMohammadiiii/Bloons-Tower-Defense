#pragma once
#include "Projectile.hpp"
#include "../balloon/Balloon.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class NormalBullet : public Projectile {
public:
    NormalBullet(sf::Vector2f start, Balloon* target);

    std::pair<bool, std::vector<std::unique_ptr<Balloon>>> update(float dt,
                std::vector<std::unique_ptr<Balloon>>& balloons,
                int& playerPoints) override;

    bool targets(const Balloon* b) const override { return b && b == m_target; }

private:
    sf::CircleShape   m_shape;
    sf::Vector2f      m_pos;
    Balloon*          m_target;
    float             m_speed{Globals::normalBullet_speed};

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
