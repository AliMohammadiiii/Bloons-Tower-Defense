#pragma once
#include "Tower.hpp"
#include "TexCache.hpp"

class IceTower : public Tower {
public:
    IceTower(sf::Vector2f pos);
    std::vector<ProjectilePtr> update(float dt,
        const std::vector<std::unique_ptr<Balloon>>& balloons) override;
private:
    sf::Sprite m_sprite;
    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
