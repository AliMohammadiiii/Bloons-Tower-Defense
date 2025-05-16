#include "Wave.hpp"


static std::mt19937 rng{std::random_device{}()};

Balloon::Kind Wave::strToKind(const std::string& s){
    if(s=="Normal")   return Balloon::Kind::Normal;
    if(s=="Pregnant") return Balloon::Kind::Pregnant;
    return Balloon::Kind::Normal;
}

Wave::Wave(const Map& m,const AttackWave& sp):m_map(m),m_gapMs(sp.enemyLaunchGapMs)
{
    for(auto& [type,cnt]: sp.enemiesCount)
        for(int i=0;i<cnt;++i) m_q.push(strToKind(type));

    std::vector<Balloon::Kind> tmp;
    while (!m_q.empty()) {             
        tmp.push_back(m_q.front());
        m_q.pop();
    }
    std::shuffle(tmp.begin(), tmp.end(), rng);  
    for (auto k : tmp) m_q.push(k);             

    std::uniform_int_distribution<int> dist(m_gapMs.first,m_gapMs.second);
    m_timer = - (dist(rng) / 1000.f);
}

std::vector<std::unique_ptr<Balloon>> Wave::update(float dt){
    std::vector<std::unique_ptr<Balloon>> out;
    if(m_done) return out;

    m_timer += dt;
    std::uniform_int_distribution<int> dist(m_gapMs.first,m_gapMs.second);

    while(!m_q.empty() && m_timer>=0.f){
        auto kind=m_q.front(); m_q.pop();

        sf::Vector2f startPx = { 0.f,0.f };
        startPx = { m_map.start().y * Globals::TILE + Globals::TILE,
                    m_map.start().x * Globals::TILE + Globals::TILE/2.f };

        if(kind==Balloon::Kind::Pregnant)
            out.push_back(std::make_unique<PregnantBalloon>(
                m_map, Balloon::Kind::Pregnant, startPx));
        else
            out.push_back(std::make_unique<Balloon>(
                m_map,kind,startPx));

        m_timer -= dist(rng)/1000.f;
    }
    if(m_q.empty() && m_timer>=0.f) m_done=true;
    return out;
}
