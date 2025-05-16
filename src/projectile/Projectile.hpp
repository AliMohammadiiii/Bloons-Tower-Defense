#pragma once
#include <SFML/Graphics.hpp>
#include "../balloon/Balloon.hpp"
#include <memory>
#include <vector>

class Projectile : public sf::Drawable {
public:
    virtual ~Projectile() = default;

    virtual bool update(float dt,
                        std::vector<std::unique_ptr<Balloon>>& balloons,
                        int& playerPoints,
                        int& playerHealth) = 0;
};
using ProjectilePtr = std::unique_ptr<Projectile>;
