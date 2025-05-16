#include "Balloon.hpp"
#include "../map/Map.hpp"
#include "../constants.hpp"
#include <cmath>
#include <map>
#include <stdexcept>

static sf::Vector2f toPx(const sf::Vector2i& t)
{
    // centre‑of‑tile in pixels
    return { t.y * Globals::TILE + Globals::TILE + Globals::TILE / 2.f,
             t.x * Globals::TILE + Globals::TILE / 2.f };
}

// -----------------------------------------------------------------------------
// texture cache helper
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

// Dedicated helper so we can build the sprite in the ctor's init‑list
static sf::Texture& texForKind(Balloon::Kind k)
{
    switch (k) {
        case Balloon::Kind::Pregnant:
            return getTexture("assets/sprites/pregnant_balloon.png");
        default:                        // Normal & Child use same red balloon
            return getTexture("assets/sprites/balloon.png");
    }
}

// -----------------------------------------------------------------------------
// ctor
Balloon::Balloon(const Map& map, Kind k, const sf::Vector2f& startPx, std::size_t startIdx)
: m_map(map)
, m_kind(k)
, m_pos(startPx)
, m_sprite(texForKind(k))     // Sprite has **no default ctor** in SFML‑3
, m_idx(startIdx)
{
    // scale sprite so the *long* side equals TILE_PIXELS
    auto sz = m_sprite.getTexture().getSize();          // Vector2u
    float s  = Globals::TILE / static_cast<float>(std::max(sz.x, sz.y));
    m_sprite.setScale(sf::Vector2f{ s, s });

    // centre origin
    m_sprite.setOrigin(sf::Vector2f{
        m_sprite.getTexture().getSize().x * 0.5f,
        m_sprite.getTexture().getSize().y * 0.5f });

    m_sprite.setPosition(m_pos);
}

// movement helpers ------------------------------------------------------------
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
        else return;                 // stay still while frozen
    }
    const auto& path = m_map.path();
    if (m_idx >= path.size()) { m_reached = true; return; }

    // Target is always current waypoint
    sf::Vector2f tgt = toPx(path[m_idx]);
    moveTowards(tgt, dt);

    // Only move to next waypoint if we've reached *this* one
    if (std::hypot(m_pos.x - tgt.x, m_pos.y - tgt.y) < 2.f) {
        ++m_idx;
        // If we're at the end after incrementing, set reachedGoal
        if (m_idx >= path.size())
            m_reached = true;
    }
}

std::vector<std::unique_ptr<Balloon>> Balloon::pop() { return {}; }

// draw ------------------------------------------------------------------------
void Balloon::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(m_sprite, st);
}

// pregnant balloon ------------------------------------------------------------
std::vector<std::unique_ptr<Balloon>> PregnantBalloon::pop()
{
    std::vector<std::unique_ptr<Balloon>> kids;


    kids.emplace_back(
        std::make_unique<Balloon>(m_map, Kind::Child, m_pos, m_idx));
    kids.emplace_back(
        std::make_unique<Balloon>(m_map, Kind::Child, m_pos, m_idx));

    return kids;
}
