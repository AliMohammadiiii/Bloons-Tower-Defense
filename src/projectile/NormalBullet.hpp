#pragma once
#include <SFML/Graphics.hpp>
#include "../balloon/Balloon.hpp"

class Projectile : public sf::Drawable {
public:
    virtual ~Projectile() = default;
    virtual bool update(float dt,
        std::vector<std::unique_ptr<Balloon>>& balloons,
        int& playerPoints, int& playerHealth) = 0;
};

class NormalBullet : public Projectile {
public:
    NormalBullet(sf::Vector2f start, Balloon* target);
    bool update(float dt,
        std::vector<std::unique_ptr<Balloon>>& balloons,
        int& playerPoints, int& playerHealth) override;

private:
    sf::CircleShape  m_shape;
    sf::Vector2f     m_pos;
    Balloon*         m_target;
    float            m_speed{300.f};
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
};
