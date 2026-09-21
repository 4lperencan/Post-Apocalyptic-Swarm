#include "Movement.hpp"

#include "Transform2D.hpp"

void Movement::Update(Transform2D& transform, const MovementState& movementState, float delta) {
    transform.position.x += movementState.moveDir.x * speed * delta;
    transform.position.y += movementState.moveDir.y * speed * delta;

    transform.rotation = movementState.aimAngle;
}
