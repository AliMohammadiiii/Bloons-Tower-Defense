#pragma once
#include "Projectile.hpp"

class BombProjectile : public Projectile {
public:
    BombProjectile(sf::Vector2f start, Balloon* target, float radius);

    bool update(float dt,
        std::vector<std::unique_ptr<Balloon>>& balloons,
        int& playerPoints,
        int& playerHealth) override;

private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_pos;
    Balloon*        m_target;
    float           m_speed{220.f};
    float           m_radius;          // explosion radius

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};
