#pragma once
#include "raylib.h"
#include "Transform2D.hpp"
#include "MovementState.hpp"

class Movement {
public:
    float speed = 200.0f;
    void Update(Transform2D& transform, const MovementState& movementState, float delta);
};