#pragma once
#include "../balloon/Balloon.hpp"
#include "../config/wave_configs.hpp"
#include "../map/Map.hpp" 
#include <queue>
#include <optional>

class Wave {
public:
    Wave(const Map&, const AttackWave& spec);

    std::vector<std::unique_ptr<Balloon>> update(float dt);
    bool finished() const { return m_done; }

private:
    const Map& m_map;
    std::queue<Balloon::Kind> m_q;
    float m_timer{0.f};
    std::pair<int,int> m_gapMs;
    bool m_done{false};

    static Balloon::Kind strToKind(const std::string&);
};
