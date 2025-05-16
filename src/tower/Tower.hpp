#pragma once
#include <SFML/Graphics.hpp>
#include <vector>               
#include <memory>
#include "../balloon/Balloon.hpp"

enum class TowerType {
    Normal,  
    Ice,     
    Bomb,    
    None    
};
class Projectile;
using ProjectilePtr = std::unique_ptr<Projectile>;

class Tower : public sf::Drawable {
public:
    Tower(sf::Vector2f pos, float range, float cooldown, int cost);
    virtual ~Tower() = default;

    bool canFire() const;
    void resetCooldown();
    virtual std::vector<ProjectilePtr> update(
        float dt,
        const std::vector<std::unique_ptr<Balloon>>& balloons) = 0;

    int cost()  const { return m_cost; }
    float range()const { return m_range; }
    bool isInRange(const sf::Vector2f& p) const;
    sf::Vector2f position() const { return m_pos; }   
protected:
    sf::Vector2f    m_pos;
    float           m_range;
    float           m_cooldown;     
    float           m_timer{0.f};
    int             m_cost;
    sf::CircleShape m_rangeCircle;
    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};
