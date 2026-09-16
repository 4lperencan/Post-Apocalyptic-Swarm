#include "GameInput.hpp"
#include "raymath.h"
#include "GameConfig.hpp"

void GameInput::Update() {
  _state.moveDir.x = (IsKeyDown(KEY_D) ? 1.0f : 0.0f) - (IsKeyDown(KEY_A) ? 1.0f : 0.0f);
  _state.moveDir.y = (IsKeyDown(KEY_S) ? 1.0f : 0.0f) - (IsKeyDown(KEY_W) ? 1.0f : 0.0f);


    if (Vector2Length( _state.moveDir) > 0)
         _state.moveDir = Vector2Normalize( _state.moveDir);

    Vector2 mousePos = GetMousePosition();
    float dx = mousePos.x - (GetScreenWidth() * 0.5f);
    float dy = mousePos.y - (GetScreenHeight() * 0.5f);
    _state.aimAngle = atan2f(dy, dx) * RAD2DEG;
    _state.shoot = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
