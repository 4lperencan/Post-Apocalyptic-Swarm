#pragma once
#include "raylib.h"

namespace GameConfig {
    inline bool SHOW_DEBUG = false;
    constexpr float OFFSCREEN_POS = -9999.0f;
    constexpr Vector2 OFFSCREEN_POSITION = { OFFSCREEN_POS, OFFSCREEN_POS};

    constexpr int BASE_W = 1280;
    constexpr int BASE_H = 720;

    inline float MAP_W = 0.0F;
    inline float MAP_H = 0.0F;


    constexpr float WALL_THICKNESS = 64.0f;

    inline bool IsInsideWall(Vector2 pos) {
        return pos.x < WALL_THICKNESS || pos.x > MAP_W - WALL_THICKNESS ||
            pos.y < WALL_THICKNESS || pos.y > MAP_H - WALL_THICKNESS;
    }

    constexpr float PLAYER_INV_TIME = 1.5f;
    constexpr int PLAYER_MAX_HEALTH = 3;
    constexpr float PLAYER_COLLIDER_RADIUS = 25.0f;
    constexpr float playerScale = 1.4f;
    constexpr float playerSpeed = 200.0f;
    constexpr Vector2 PLAYER_PIVOT = {0.31f, 0.58f};
    constexpr Vector2 PLAYER_MUZZLE_OFFSET = {53.0f, 0.1f};


    //Dalgalar
    constexpr int WAVE_ENEMY_BASE = 8;
    constexpr int WAVE_ENEMY_RAMP = 4;

    constexpr int SCORE_PER_KILL = 10;


}