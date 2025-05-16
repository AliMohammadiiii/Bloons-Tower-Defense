#pragma once
#include "Tower.hpp"
#include "TexCache.hpp"
#include "../projectile/NormalBullet.hpp"
#include "../constants.hpp"
#include "TexCache.hpp"
class NormalTower : public Tower {
public:
    NormalTower(sf::Vector2f pos);
    std::vector<ProjectilePtr> update(float dt,
        const std::vector<std::unique_ptr<Balloon>>& balloons) override;
private:
    sf::Sprite m_sprite;
    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
