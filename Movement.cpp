#include "Movement.hpp"

#include "Transform2D.hpp"

void Movement::Update(Transform2D& transform, const MovementState& movementState, float delta) {
    transform.position.x += (float) movementState.moveDir.x * delta;
    transform.position.y += (float) movementState.moveDir.y * delta;

    transform.rotation = movementState.aimAngle;
}
