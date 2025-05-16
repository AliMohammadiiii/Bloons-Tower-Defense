#include "NormalTower.hpp"
#include "../projectile/NormalBullet.hpp"
#include "../constants.hpp"
#include "TexCache.hpp"

NormalTower::NormalTower(sf::Vector2f p)
: Tower   (p, 100.f, 0.5f, 10)
, m_sprite(getTowerTex("assets/sprites/normal.png"))   
{
    float s = Globals::TILE /
              static_cast<float>(m_sprite.getTexture().getSize().x);
    m_sprite.setScale(sf::Vector2f{s, s});
    m_sprite.setOrigin(sf::Vector2f{
        m_sprite.getTexture().getSize().x * 0.5f,
        m_sprite.getTexture().getSize().y * 0.5f });
    m_sprite.setPosition(m_pos);
}

void NormalTower::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_sprite, st);
    Tower::draw(rt, st);  
}



std::vector<ProjectilePtr> NormalTower::update(
    float dt,
    const std::vector<std::unique_ptr<Balloon>>& balloons)
{
    m_timer += dt;
    if (!canFire()) return {};
    Balloon* target = nullptr;
    float bestDist = m_range * m_range;
    for (auto& b : balloons) {
        if (!b->reachedGoal() && !b->isDead()) {
            float d2 = (b->getPosition() - m_pos).x*(b->getPosition()-m_pos).x
                     + (b->getPosition() - m_pos).y*(b->getPosition()-m_pos).y;
            if (d2 < bestDist) {
                bestDist = d2;
                target = b.get();
            }
        }
    }
    if (!target) return {};

    resetCooldown();
    std::vector<ProjectilePtr> shots;
    shots.push_back(std::make_unique<NormalBullet>(m_pos, target));
    return shots;
}
