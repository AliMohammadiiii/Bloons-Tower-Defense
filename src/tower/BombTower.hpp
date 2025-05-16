#pragma once
#include "Tower.hpp"

class BombTower : public Tower {
public:
    BombTower(sf::Vector2f pos);
    std::vector<ProjectilePtr> update(float dt,
        const std::vector<std::unique_ptr<Balloon>>& balloons) override;

private:
    float m_explosionRadius{60.f};
private:
    sf::Sprite m_sprite;
    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
