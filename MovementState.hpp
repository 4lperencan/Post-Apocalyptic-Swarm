#pragma once
#include "raylib.h"
#include "Transform2D.hpp"


struct MovementState {
    Vector2 moveDir = {0.0f,0.0f};
    float aimAngle = 0.0f;
    bool shoot = false;
};