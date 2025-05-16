#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>

class Map;

class Balloon : public sf::Drawable {
public:
    enum class Kind { Normal, Pregnant, Child };

    Balloon(const Map&, Kind, const sf::Vector2f& startPx, std::size_t startIdx = 0);
    void setPathIndex(std::size_t i) { m_idx = i; }
    void update(float dt);
    bool reachedGoal() const { return m_reached; }
    Kind kind() const { return m_kind; }
    virtual int score() const { return 1; }

    virtual std::vector<std::unique_ptr<Balloon>> pop();
    sf::Vector2f getPosition() const { return m_pos; }
    float        getRadius()   const {
        auto bounds = m_sprite.getGlobalBounds();
        return bounds.size.x * 0.5f;   
    }
    void freeze(float seconds) { m_frozen = true; m_freezeTimer = seconds; }
    bool isFrozen() const      { return m_frozen; }
    void kill()                { m_dead = true; }
    bool  isDead() const      { return m_dead; }

    void  goal()  { m_reachedGoal = true; }  

protected:  
    const Map& m_map;
    Kind       m_kind;
    std::size_t m_idx{1};
    sf::Vector2f m_pos;
    bool m_reached{false};
    sf::Sprite m_sprite;
    bool  m_frozen{false};
    float m_freezeTimer{0.f};
    bool m_dead{false};
    bool m_reachedGoal{false};
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
    void moveTowards(const sf::Vector2f& target,float dt);
};

class PregnantBalloon : public Balloon {
public:
    using Balloon::Balloon;
    int score() const override { return 3; }
    std::vector<std::unique_ptr<Balloon>> pop() override;
};
