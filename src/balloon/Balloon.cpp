#include "Balloon.hpp"
#include "../map/Map.hpp"
#include "../constants.hpp"
#include <cmath>
#include <map>
#include <stdexcept>

static sf::Vector2f toPx(const sf::Vector2i& t)
{
    return { t.y * Globals::TILE + Globals::TILE / 4.f,
             t.x * Globals::TILE + Globals::TILE / 2.f };
}

static sf::Texture& getTexture(const std::string& path)
{
    static std::map<std::string, sf::Texture> cache;
    auto it = cache.find(path);
    if (it != cache.end()) return it->second;

    sf::Texture tex;
    if (!tex.loadFromFile(path))
        throw std::runtime_error("Failed to load texture: " + path);

    return cache.emplace(path, std::move(tex)).first->second;
}

static sf::Texture& texForKind(Balloon::Kind k)
{
    switch (k) {
        case Balloon::Kind::Pregnant:
            return getTexture("assets/sprites/pregnant_balloon.png");
        default:                      
            return getTexture("assets/sprites/balloon.png");
    }
}

Balloon::Balloon(const Map& map, Kind k, const sf::Vector2f& startPx, std::size_t startIdx)
: m_map(map)
, m_kind(k)
, m_idx(startIdx)
, m_pos(startPx)
, m_sprite(texForKind(k))    
{
    auto sz = m_sprite.getTexture().getSize();          
    float s  = Globals::TILE / static_cast<float>(std::max(sz.x, sz.y));
    m_sprite.setScale(sf::Vector2f{ s, s });

    m_sprite.setOrigin(sf::Vector2f{
        m_sprite.getTexture().getSize().x * 0.5f - Globals::TILE,
        m_sprite.getTexture().getSize().y * 0.5f });
    m_pos.x -= Globals::TILE;
    m_sprite.setPosition(m_pos);
}

void Balloon::moveTowards(const sf::Vector2f& tgt, float dt)
{
    sf::Vector2f d = tgt - m_pos;
    float len = std::hypot(d.x, d.y);
    if (len < 1e-4f) return;
    d /= len;
    m_pos += d * BALLOON_SPEED * dt;
    m_sprite.setPosition(m_pos);
}

void Balloon::update(float dt)
{

    if (m_reached) return;
    if (m_frozen) {
        m_freezeTimer -= dt;
        if (m_freezeTimer <= 0.f) m_frozen = false;
        else return;                 
    }
    const auto& path = m_map.path();
    if (m_idx >= path.size()) { m_reached = true; return; }

    sf::Vector2f tgt = toPx(path[m_idx]);
    moveTowards(tgt, dt);

    if (std::hypot(m_pos.x - tgt.x, m_pos.y - tgt.y) < 2.f) {
        ++m_idx;
        if (m_idx >= path.size())
            m_reached = true;
    }
}

std::vector<std::unique_ptr<Balloon>> Balloon::pop() { return {}; }

void Balloon::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_sprite, st);
}

std::vector<std::unique_ptr<Balloon>> PregnantBalloon::pop()
{
    std::vector<std::unique_ptr<Balloon>> kids;

    m_pos.x += Globals::TILE;
    kids.emplace_back(
        std::make_unique<Balloon>(m_map, Kind::Child, m_pos, m_idx));
    kids.emplace_back(
        std::make_unique<Balloon>(m_map, Kind::Child, m_pos, m_idx));

    return kids;
}
