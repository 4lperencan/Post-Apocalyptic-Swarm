#include "Enemy.hpp"
#include "ResourceKeys.hpp"

Enemy::Enemy() {
    _sprite.Init(RK::ZOMBIE_MOVE, 60,60, 6, 10.0f);
    _sprite.rotationOffset = 90.0f;
    _transform.scale = 0.8f;
    _transform.rotation = 180.0f;
}


void Enemy::Update(float dt) {
    _transform.MoveForward(_speed * dt);
    _sprite.Update(dt);
}
void Enemy::Draw() {
    _sprite.Draw(_transform);
}
void Enemy::SetPosition(Vector2 position) {
    _transform.position = position;
}