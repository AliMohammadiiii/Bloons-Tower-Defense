#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "../map/Map.hpp"
#include "../renderer/MapRenderer.hpp"
#include "../logic/Wave.hpp"
#include "../tower/Tower.hpp"
#include "../projectile/Projectile.hpp"
#include "../ui/TowerPanel.hpp"
#include "../ui/Hud.hpp"

class Game {
public:
    explicit Game(const std::string& mapFile);
    void run();

private:
    // ------------ world ------------
    Map          m_map;
    MapRenderer  m_renderer;
    sf::RenderWindow m_win;
    
    // ------------ timing ------------
    sf::Clock m_clk;                     // frame timer

    // ------------ waves ------------
    std::vector<Wave> m_waves;
    std::size_t       m_waveIdx      {0};
    float             m_nextWaveTime {0.f};

    // ------------ entities ------------
    std::vector<std::unique_ptr<Balloon>>   m_balloons;
    std::vector<std::unique_ptr<Tower>>     m_towers;
    std::vector<ProjectilePtr>              m_projectiles;

    // ------------ UI / player state ------------
    int         m_playerPoints  {100};
    int         m_playerHealth  {20};
    TowerType   m_pendingPlacement {TowerType::None};
    TowerPanel  m_panel;    // must come AFTER m_playerPoints (ctor needs ref)
    Hud m_hud { m_playerHealth, m_playerPoints };
    // ------------ helpers ------------
    void handleEvents();
    void update(float dt);
};
