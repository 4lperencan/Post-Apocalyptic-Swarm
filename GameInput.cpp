#include "GameInput.hpp"
#include "raymath.h"
#include "GameConfig.hpp"

void GameInput::Update(const Camera2D& camera, Vector2 aimOrigin) {
  _state.moveDir.x = (IsKeyDown(KEY_D) ? 1.0f : 0.0f) - (IsKeyDown(KEY_A) ? 1.0f : 0.0f);
  _state.moveDir.y = (IsKeyDown(KEY_S) ? 1.0f : 0.0f) - (IsKeyDown(KEY_W) ? 1.0f : 0.0f);


    if (Vector2Length( _state.moveDir) > 0)
         _state.moveDir = Vector2Normalize( _state.moveDir);

    Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 toMouse = Vector2Subtract(mouseWorld, aimOrigin);
    _state.aimAngle = atan2f(toMouse.y, toMouse.x) * RAD2DEG;


    _state.shoot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
