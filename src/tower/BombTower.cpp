#include "BombTower.hpp"
#include "../projectile/BombProjectile.hpp"
#include "TexCache.hpp"
#include "../constants.hpp"

BombTower::BombTower(sf::Vector2f p)
: Tower(p, 140.f, 1.2f, 30)
, m_sprite(getTowerTex("assets/sprites/bomb.png"))
{
    m_rangeCircle.setOutlineColor(sf::Color::Yellow);

    float s = Globals::TILE / static_cast<float>(m_sprite.getTexture().getSize().x);
    m_sprite.setScale(sf::Vector2f{s, s});
    m_sprite.setOrigin(sf::Vector2f{
        m_sprite.getTexture().getSize().x / 2.f,
        m_sprite.getTexture().getSize().y / 2.f});
    m_sprite.setPosition(m_pos);
}
void BombTower::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_sprite, st);
    Tower::draw(rt, st);  
}
std::vector<ProjectilePtr> BombTower::update(
    float dt,
    const std::vector<std::unique_ptr<Balloon>>& balloons)
{
    m_timer += dt;
    if (!canFire()) return {};

    Balloon* bestTarget=nullptr;
    std::size_t bestCount=0;

    for (auto& cand : balloons) {
        if (cand->reachedGoal() or cand->isDead()) continue;
        if (!isInRange(cand->getPosition())) continue;
        
        std::size_t count=0;
        for (auto& b : balloons) {
            float d = std::hypot(b->getPosition().x - cand->getPosition().x,
                                 b->getPosition().y - cand->getPosition().y);
            if (d <= m_explosionRadius) ++count;
        }
        if (count > bestCount) {
            bestCount=count; bestTarget=cand.get();
        }
    }

    if (!bestTarget) return {};

    resetCooldown();
    std::vector<ProjectilePtr> v;
    v.push_back(std::make_unique<BombProjectile>(
        m_pos, bestTarget, m_explosionRadius));
    return v;
}
