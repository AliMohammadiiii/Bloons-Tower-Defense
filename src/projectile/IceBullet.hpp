#pragma once
#include "Projectile.hpp"

class IceBullet : public Projectile {
public:
    IceBullet(sf::Vector2f start, Balloon* target);

    bool update(float dt,
        std::vector<std::unique_ptr<Balloon>>& balloons,
        int& playerPoints,
        int& playerHealth) override;

private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_pos;
    Balloon*        m_target;
    float           m_speed{280.f};

    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};
