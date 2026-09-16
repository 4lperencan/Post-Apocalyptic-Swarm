#include "Player.hpp"
#include "GameConfig.hpp"
#include "GameInput.hpp"
#include "raymath.h"


Player::Player(const std::string& textureName) {
    _sprite.pivot = GameConfig::PLAYER_PIVOT;
    _sprite.Init(textureName);
    _transform.scale = GameConfig::playerScale;
    _movement.speed = GameConfig::playerSpeed;
    _muzzleOffset = GameConfig::PLAYER_MUZZLE_OFFSET;
}
void Player::Update(float delta)
{
    _movement.Update(_transform, GI::get().State(), delta);
}


void  Player::SetPosition(Vector2 position) {
    _transform.position = position;
}
Vector2 Player::GetPosition() const {
    return _transform.position;
}

Vector2 Player::GetFiringPosition() const {
    float rad = _transform.rotation * DEG2RAD; // DEG2RAD means pi / 180 note for myself!!
    Vector2 rotated = Vector2Rotate(_muzzleOffset, rad);
    return Vector2Add(_transform.position, rotated);
}


void Player::Draw() const {
    _sprite.Draw(_transform);
}