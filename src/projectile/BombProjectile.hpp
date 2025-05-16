#pragma once
#include "Projectile.hpp"
#include "../balloon/Balloon.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class BombProjectile : public Projectile {
public:
    BombProjectile(sf::Vector2f start, Balloon* target, float explosionRadius);

    std::pair<bool, std::vector<std::unique_ptr<Balloon>>> update(float dt,
                std::vector<std::unique_ptr<Balloon>>& balloons,
                int& playerPoints) override;

    bool targets(const Balloon* b) const override { return b && b == m_target; }

private:
    sf::CircleShape   m_shape;
    sf::Vector2f      m_pos;
    Balloon*          m_target;
    float             m_speed{220.f};
    float             m_radius;      

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
