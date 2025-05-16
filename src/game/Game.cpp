#include "Game.hpp"
#include <iostream>
#include "../config/wave_configs.hpp"
#include "../tower/NormalTower.hpp"
#include "../tower/IceTower.hpp"
#include "../tower/BombTower.hpp"
#include "../constants.hpp"      // for Globals::TILE
// src/game/Game.cpp

Game::Game(const std::string& mapFile)
: m_map(mapFile)
, m_renderer(m_map, "assets/tiles/grass.jpg", "assets/tiles/road.jpg", {1080, 720})
, m_win(sf::VideoMode(sf::Vector2u{1080,720}), "Bloons TD")
, m_panel(200, 720, m_playerPoints)  // depends on m_playerPoints
{
    m_win.setFramerateLimit(60);
    
    // Build waves from config
    for (const auto& spec : ATTACKING_PLAN)
        m_waves.emplace_back(m_map, spec);

    // First wave launches after this many seconds
    m_nextWaveTime = WAVE_LAUNCH_GAP_SECS;
}

void Game::run()
{
    while (m_win.isOpen())
    {
        handleEvents();

        float dt = m_clk.restart().asSeconds();
        update(dt);

        m_win.clear();
        m_renderer.draw(m_win);

        // Draw towers, projectiles, balloons
        for (auto& t : m_towers)        m_win.draw(*t);
        for (auto& p : m_projectiles)   m_win.draw(*p);
        for (auto& b : m_balloons)      m_win.draw(*b);

        // Draw the UI panel last
        m_panel.draw(m_win);
        m_hud.draw(m_win);
        m_win.display();
    }
}

void Game::handleEvents()
{
    // SFML 3: pollEvent returns std::optional<sf::Event>
    while (auto evOpt = m_win.pollEvent())
    {
        const sf::Event& ev = *evOpt;

        // Window close
        if (ev.is<sf::Event::Closed>())
        {
            m_win.close();
            return;
        }

        // Mouse click
        if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mb->button == sf::Mouse::Button::Left)
            {
                sf::Vector2i mousePx = sf::Mouse::getPosition(m_win);

                // 1) Click in panel? Select tower type.
                if (auto type = m_panel.handleClick(mousePx); type != TowerType::None)
                {
                std::cout << "Mouse clicked" << std::endl;
                    m_pendingPlacement = type;
                    return;
                }

                // 2) Else if a tower is selected, try placing it
                if (m_pendingPlacement != TowerType::None)
                {
                    float padX = m_renderer.leftPad();
                    float padY = m_renderer.topPad();

                    int col = int((mousePx.x - padX) / Globals::TILE);
                    int row = int((mousePx.y - padY) / Globals::TILE);
                    std::cout << "Click row="<<row<<" col="<<col<<"\n";
                    if (mousePx.x < padX || mousePx.x > 1080 - 200)   // panel is 200 px wide
                        return;                                       // ignore click
                    if (mousePx.y < padY)
                        return;
                    if (row >= 0 && row < (int)m_map.grid().size() &&
                        col >= 0 && col < (int)m_map.grid()[0].size() &&
                        m_map.at(row, col) == TileType::Buildable)
                    {
                        // Center of the tile
                        sf::Vector2f pos {
                            col * Globals::TILE + Globals::TILE/2.f,
                            row * Globals::TILE + Globals::TILE/2.f
                        };

                        // Create the right tower
                        std::unique_ptr<Tower> t;
                        switch (m_pendingPlacement)
                        {
                            case TowerType::Normal:
                                t = std::make_unique<NormalTower>(pos);
                                break;
                            case TowerType::Ice:
                                t = std::make_unique<IceTower>(pos);
                                break;
                            case TowerType::Bomb:
                                t = std::make_unique<BombTower>(pos);
                                break;
                            default:
                                break;
                        }
                        std::cout << m_playerPoints << std::endl;
                        // If affordable, place it
                        if (t && m_playerPoints >= t->cost())
                        {
                            m_playerPoints -= t->cost();
                            m_towers.push_back(std::move(t));
                        }
                    }

                    // Reset selection
                    m_pendingPlacement = TowerType::None;
                }
            }
        }
    }
}

void Game::update(float dt)
{
    // Update the UI panel (gray-out icons)
    m_panel.update();

    // Global timer for launching waves
    static float globalTime = 0.f;
    globalTime += dt;

    if (m_waveIdx < m_waves.size() && globalTime >= m_nextWaveTime)
    {
        // Schedule next wave launch
        m_nextWaveTime += WAVE_LAUNCH_GAP_SECS;
    }

    // Spawn from current wave
    if (m_waveIdx < m_waves.size())
    {
        auto spawned = m_waves[m_waveIdx].update(dt);
        for (auto& b : spawned)
            m_balloons.push_back(std::move(b));

        if (m_waves[m_waveIdx].finished())
            ++m_waveIdx;
    }

    // Update towers → generate new projectiles
    for (auto& t : m_towers)
    {
        auto shots = t->update(dt, m_balloons, m_playerPoints);
        for (auto& s : shots)
            m_projectiles.push_back(std::move(s));
    }

    // Update projectiles → remove those that expire or hit
    for (auto it = m_projectiles.begin(); it != m_projectiles.end(); )
    {
        if ((*it)->update(dt, m_balloons, m_playerPoints, m_playerHealth))
            it = m_projectiles.erase(it);
        else
            ++it;
    }
    
    // Update balloons → remove those that reach the goal
    // for (auto it = m_balloons.begin(); it != m_balloons.end(); )
    // {
    //     (*it)->update(dt);
        
        
    //     if ((*it)->reachedGoal())
    //     {
    //         --m_playerHealth;
    //         it = m_balloons.erase(it);
    //     }
    //     else if ((*it)->isDead())
    //     {
    //         it = m_balloons.erase(it);
    //     }
    //     else
    //     {
    //         ++it;
    //     }
    // }
    // for (auto it = m_balloons.begin(); it != m_balloons.end(); ) {
    //     (*it)->update(dt);

    //     if ((*it)->isDead()) {
    //         it = m_balloons.erase(it); // advance using erase return
    //     } else if ((*it)->reachedGoal()) {
    //         --m_playerHealth;
    //         it = m_balloons.erase(it); // also advance
    //     } else {
    //         ++it; // normal advancement
    //     }
    // }
    for (auto& b : m_balloons) b->update(dt);
    m_balloons.erase(
        std::remove_if(
            m_balloons.begin(), m_balloons.end(),
            [&](const auto& b){
                if (b->isDead())           return true;          // popped
                if (b->reachedGoal()){ --m_playerHealth; return true; }
                return false;
            }),
        m_balloons.end());
    m_hud.update();

    // End-game checks
    if (m_playerHealth <= 0)
    {
        std::cout << "You Lose!\n";
        m_win.close();
    }
    else if (m_waveIdx >= m_waves.size() && m_balloons.empty())
    {
        std::cout << "You Win!\n";
        m_win.close();
    }
    
}
