#pragma once
inline constexpr float TILE_PIXELS   = 48.f;   // size of one map tile (px)
inline constexpr float BALLOON_SPEED = 60.f;   // px per second
namespace Globals {
    inline float TILE = 48.f;   // overwritten by MapRenderer at runtime
}