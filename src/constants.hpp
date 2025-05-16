#pragma once
#include <string>
inline constexpr float TILE_PIXELS   = 48.f;  
inline constexpr float BALLOON_SPEED = 60.f;   
namespace Globals {
    inline float TILE = 48.f;   
    inline int player_init_score = 100;
    inline int player_health = 15;
    inline const int costs_tower[3] = {50, 65, 100};
    inline const unsigned screen_size[2] = {1080, 720};
    inline const float screen_sizef[2] = {1080.f, 720.f};
    inline const int panel_size = 200;
    inline const int frame_rate = 60;
    inline const float bomb_speed = 220.f;
    inline const float bomb_range = 140.f;
    inline const float bomb_cooldown = 1.2f;
    inline const float bomb_explosionRadius = 60.f;
    inline const float ice_speed = 110.f;
    inline const float ice_range = 0.8f;
    inline const float ice_cooldown = 1.2f;
    inline const float ice_freeze_time = 0.2f;
    inline const float normalBullet_speed = 300.f;
    inline const float normalBullet_range = 100.f;
    inline const float normalBullet_cooldown = 0.5f;
}

namespace Assets {
    inline std::string FontSF = "assets/fonts/OpenSans.ttf";
    inline std::string GrassPath = "assets/tiles/grass.jpg";
    inline std::string RoadPath = "assets/tiles/road.jpg";
    inline std::string BalloonsPath = "assets/sprites/balloon.png";
    inline std::string PregnantBalloonsPath = "assets/sprites/pregnant_balloon.png";
    inline std::string NormalTowerPath = "assets/sprites/normal.png";
    inline std::string IceTowerPath = "assets/sprites/ice.png";
    inline std::string BombTowerPath = "assets/sprites/bomb.png";
    inline int scoreBalloons = 1;
    inline int scorePregnantBalloons = 3;
}

namespace MAP {
    inline const char Start = 'S';
    inline const char Finish = 'F';
    inline const char Path = 'O';
}