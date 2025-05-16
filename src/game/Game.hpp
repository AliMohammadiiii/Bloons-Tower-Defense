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

#include <iostream>
#include "../config/wave_configs.hpp"
#include "../tower/NormalTower.hpp"
#include "../tower/IceTower.hpp"
#include "../tower/BombTower.hpp"
#include "../constants.hpp"

#include <iostream>
#include <algorithm>
#include <SFML/Audio.hpp>

class Game {
public:
    explicit Game(const std::string& mapFile);
    void run();

private:
    Map           m_map;
    MapRenderer   m_renderer;
    sf::RenderWindow m_win;
    sf::Music m_music;
    
    sf::Clock m_clk;

    std::vector<Wave> m_waves;
    std::size_t       m_waveIdx      {0};
    float             m_nextWaveTime;

    std::vector<std::unique_ptr<Balloon>>   m_balloons;
    std::vector<std::unique_ptr<Tower>>     m_towers;
    std::vector<ProjectilePtr>              m_projectiles;

    int        m_playerPoints;     
    int        m_playerHealth;
    TowerType  m_pendingPlacement {TowerType::None};

    TowerPanel m_panel {Globals::panel_size, Globals::screen_size[1], m_playerPoints};
    Hud        m_hud   {m_playerHealth, m_playerPoints};

    bool     m_gameOver {false};
    sf::Text m_endText;                 

    void handleEvents();
    void update(float dt);
};
