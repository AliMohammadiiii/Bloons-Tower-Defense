#pragma once
#include <SFML/Graphics.hpp>
#include "../balloon/Balloon.hpp"
#include <memory>
#include <vector>

class Projectile : public sf::Drawable {
public:
    virtual ~Projectile() = default;

    virtual std::pair<bool, std::vector<std::unique_ptr<Balloon>>> update(float dt,
                        std::vector<std::unique_ptr<Balloon>>& balloons,
                        int& playerPoints) = 0;
    virtual bool targets(const Balloon* b) const = 0;
};
using ProjectilePtr = std::unique_ptr<Projectile>;
