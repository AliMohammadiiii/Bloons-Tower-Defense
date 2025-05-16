#include "IceTower.hpp"


IceTower::IceTower(sf::Vector2f p)
: Tower(p, Globals::ice_speed, Globals::ice_cooldown, Globals::costs_tower[1])
, m_sprite(getTowerTex(Assets::IceTowerPath))
{
    m_rangeCircle.setOutlineColor(sf::Color::Cyan);

    float s = Globals::TILE / static_cast<float>(m_sprite.getTexture().getSize().x);
    m_sprite.setScale(sf::Vector2f{s, s});
    m_sprite.setOrigin(sf::Vector2f{
        m_sprite.getTexture().getSize().x / 2.f,
        m_sprite.getTexture().getSize().y / 2.f});
    m_sprite.setPosition(m_pos);
}
void IceTower::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_sprite, st);
    Tower::draw(rt, st);  
}

std::vector<ProjectilePtr> IceTower::update(
    float dt,
    const std::vector<std::unique_ptr<Balloon>>& balloons)
{
    m_timer += dt;
    if (!canFire()) return {};

    Balloon* target=nullptr;
    float best=range()*range();
    for (auto& b : balloons) {
        if (!b->reachedGoal() && !b->isFrozen()) {
            float d2 = (b->getPosition()-m_pos).x*(b->getPosition()-m_pos).x +
                       (b->getPosition()-m_pos).y*(b->getPosition()-m_pos).y;
            if (d2 < best) {
                best=d2; target=b.get();
            }
        }
    }
    if (!target) return {};

    resetCooldown();
    std::vector<ProjectilePtr> out;
    out.push_back(std::make_unique<IceBullet>(m_pos, target));
    return out;
}
