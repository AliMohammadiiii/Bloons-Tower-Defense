
#include "Game.hpp"

static sf::Font& hudFont()
{
    static sf::Font font;
    static bool ok = font.openFromFile(Assets::FontSF);
    if (!ok) throw std::runtime_error("missing font");
    return font;
}

Game::Game(const std::string& mapFile)
: m_map      (mapFile)
, m_renderer (m_map,
              Assets::GrassPath,
              Assets::RoadPath,
              {Globals::screen_size[0], Globals::screen_size[1]},
              Globals::panel_size)
, m_win      (sf::VideoMode(sf::Vector2u{Globals::screen_size[0], Globals::screen_size[1]}), "Bloons TD")
, m_endText  (hudFont(), "", Globals::frame_rate)           
{
    m_win.setFramerateLimit(Globals::frame_rate);

    for (const auto& spec : ATTACKING_PLAN)
        m_waves.emplace_back(m_map, spec);

    m_nextWaveTime = WAVE_LAUNCH_GAP_SECS;

    m_endText.setFillColor(sf::Color::White);
    m_endText.setPosition(sf::Vector2f{Globals::screen_sizef[0] / 2,Globals::screen_sizef[1] / 2}); 
    m_nextWaveTime = WAVE_LAUNCH_GAP_SECS;
    m_playerHealth = Globals::player_health;
    m_playerPoints = Globals::player_init_score;
    // if (!m_music.loadFromFile("assets/music/epic.mp3")) {
    //     throw std::runtime_error("Failed to load background music!");
    // }
    m_music = sf::Music("assets/music/epic.mp3");
    m_music.setLooping(true);
    m_music.setVolume(50.f);      
    m_music.play();              
}

void Game::run()
{
    while (m_win.isOpen())
    {
        handleEvents();

        float dt = m_clk.restart().asSeconds();
        if (!m_gameOver) update(dt);
        m_hud.update();

        m_win.clear();
        m_renderer.draw(m_win);

        for (auto& t : m_towers)      m_win.draw(*t);
        for (auto& p : m_projectiles) m_win.draw(*p);
        for (auto& b : m_balloons)    m_win.draw(*b);

        m_panel.draw(m_win);
        m_hud.draw(m_win);

        if (m_gameOver) {
            sf::RectangleShape fade(sf::Vector2f{Globals::screen_sizef[0], Globals::screen_sizef[1]});
            fade.setFillColor(sf::Color(0,0,0,150));
            m_win.draw(fade);
            m_win.draw(m_endText);
        }
        m_win.display();
    }
}

void Game::handleEvents()
{
    while (auto evOpt = m_win.pollEvent())
    {
        const sf::Event& ev = *evOpt;

        if (ev.is<sf::Event::Closed>()) { m_win.close(); return; }

        if (m_gameOver) {
            if (const auto* kp = ev.getIf<sf::Event::KeyPressed>())
                if (kp->code == sf::Keyboard::Key::Escape) m_win.close();
            continue;
        }

        if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>())
            if (mb->button == sf::Mouse::Button::Left)
            {
                sf::Vector2i mousePx = sf::Mouse::getPosition(m_win);

                if (auto type = m_panel.handleClick(mousePx);
                    type != TowerType::None)
                { m_pendingPlacement = type; return; }

                if (m_pendingPlacement == TowerType::None) return;

                float padX = m_renderer.leftPad();
                float padY = m_renderer.topPad();

                if (mousePx.x < padX || mousePx.x > Globals::screen_sizef[0]-Globals::panel_size) return;
                if (mousePx.y < padY)                         return;

                int col = int((mousePx.x - padX) / Globals::TILE);
                int row = int((mousePx.y - padY) / Globals::TILE);

                if (row<0 || row>=int(m_map.grid().size()) ||
                    col<0 || col>=int(m_map.grid()[0].size())) return;

                if (m_map.at(row,col) != TileType::Buildable) return;

                for (const auto& tw : m_towers) {
                    sf::Vector2f tp = tw->position();
                    int tCol = int((tp.x - padX)/Globals::TILE);
                    int tRow = int((tp.y - padY)/Globals::TILE);
                    if (tCol == col && tRow == row) {
                        m_pendingPlacement = TowerType::None;
                        return;
                    }
                }

                sf::Vector2f pos{
                    col*Globals::TILE + Globals::TILE/2.f,
                    row*Globals::TILE + Globals::TILE/2.f };

                std::unique_ptr<Tower> t;
                switch (m_pendingPlacement) {
                    case TowerType::Normal: t = std::make_unique<NormalTower>(pos); break;
                    case TowerType::Ice:    t = std::make_unique<IceTower>(pos);    break;
                    case TowerType::Bomb:   t = std::make_unique<BombTower>(pos);   break;
                    default: break;
                }

                if (t && m_playerPoints >= t->cost()) {
                    m_playerPoints -= t->cost();
                    m_towers.push_back(std::move(t));
                }
                m_pendingPlacement = TowerType::None;
            }
    }
}
void Game::update(float dt)
{
    static float gTime = 0.f; 
    gTime += dt;

    if (m_waveIdx < m_waves.size() && gTime >= m_nextWaveTime) {
        auto spawned = m_waves[m_waveIdx].update(dt);
        for (auto& b : spawned) m_balloons.push_back(std::move(b));
        if (m_waves[m_waveIdx].finished())
        {
            ++m_waveIdx;
            m_nextWaveTime = gTime + WAVE_LAUNCH_GAP_SECS;
        } 
    }

    for (auto& t : m_towers) {
        auto shots = t->update(dt, m_balloons);
        for (auto& s : shots) m_projectiles.push_back(std::move(s));
    }

    std::vector<std::unique_ptr<Balloon>> newKids;

    for (auto it = m_projectiles.begin(); it != m_projectiles.end(); )
    {
        auto [removeIt, spawned] =
            (*it)->update(dt, m_balloons, m_playerPoints);

        for (auto& k : spawned) newKids.push_back(std::move(k));

        if (removeIt) it = m_projectiles.erase(it);
        else          ++it;
    }

    m_balloons.insert(
        m_balloons.end(),
        std::make_move_iterator(newKids.begin()),
        std::make_move_iterator(newKids.end()));

    
    for (auto& b : m_balloons)
        b->update(dt);


    m_balloons.erase(
        std::remove_if(
            m_balloons.begin(), m_balloons.end(),
            [&](const std::unique_ptr<Balloon>& bPtr) {
                Balloon* b = bPtr.get();

                if (b->isDead() || b->reachedGoal())
                {
                    for (auto& pr : m_projectiles)
                        if (pr->targets(b))
                            return false;   

                    if (b->reachedGoal())
                        --m_playerHealth;
                    return true;         
                }
                return false; 
            }),
        m_balloons.end()
    );
    m_panel.update();
    if (!m_gameOver && m_playerHealth <= 0) {
        m_endText.setString("YOU LOSE!");
        auto b = m_endText.getLocalBounds();
       m_endText.setOrigin(sf::Vector2f{ b.size.x * 0.5f, b.size.y * 0.5f });
        m_gameOver = true;
    }
    if (!m_gameOver &&
        m_waveIdx >= m_waves.size() && m_balloons.empty())
    {
        m_endText.setString("YOU WIN!");
        auto b = m_endText.getLocalBounds();
        m_endText.setOrigin(sf::Vector2f{ b.size.x * 0.5f, b.size.y * 0.5f });
        m_gameOver = true;
    }
}
