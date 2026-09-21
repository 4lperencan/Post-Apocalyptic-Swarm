#pragma once
#include "raylib.h"

namespace GameConfig {
    inline bool SHOW_DEBUG = false;
    constexpr float OFFSCREEN_POS = -9999.0f;
    constexpr Vector2 OFFSCREEN_POSITION = { OFFSCREEN_POS, OFFSCREEN_POS};

    constexpr float BOUNDS_MARGIN = 100.0f;
    constexpr int BASE_W = 1280;
    constexpr int BASE_H = 720;

    inline float MAP_W = 0.0F;
    inline float MAP_H = 0.0F;


    inline bool IsOutSideMap(Vector2 pos) {
        return pos.x < -BOUNDS_MARGIN || pos.x > MAP_W + BOUNDS_MARGIN ||
            pos.y < -BOUNDS_MARGIN || pos.y > MAP_H + BOUNDS_MARGIN;
    }


    constexpr float WALL_THICKNESS = 64.0f;

    inline bool IsInsideWall(Vector2 pos) {
        return pos.x < WALL_THICKNESS || pos.x > MAP_W - WALL_THICKNESS ||
            pos.y < WALL_THICKNESS || pos.y > MAP_H - WALL_THICKNESS;
    }


    constexpr float playerScale = 1.4f;
    constexpr float playerSpeed = 200.0f;
    constexpr float AIM_SENSITIVITY = 0.15f;
    constexpr Vector2 PLAYER_PIVOT = {0.31f, 0.58f};
    constexpr Vector2 PLAYER_MUZZLE_OFFSET = {53.0f, 0.1f};

}