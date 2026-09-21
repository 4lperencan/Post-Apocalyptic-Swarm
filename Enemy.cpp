#include "Enemy.hpp"

#include "GameConfig.hpp"
#include "ResourceKeys.hpp"
#include "Player.hpp"

Enemy::Enemy() {
    _spriteMove.Init(RK::ZOMBIE_MOVE, 60,60, 6, 8.0f);
    _spriteMove.rotationOffset = 90.0f;
    _spriteDeath.Init(RK::ZOMBIE_DEATH, 128,112, 8, 8.0f, false);
    _spriteDeath.rotationOffset = 90.0f;
    _transform.scale = 0.8f;
    _transform.rotation = 0.0f;
    _collider.Init(30.0f, _transform);
}

void Enemy::Kill() {
    if (_state == EnemyState::Dying) return;
    _state = EnemyState::Dying;

    _spriteDeath.Reset();
    TraceLog(LOG_INFO,"Enemy Killed");
}


void Enemy::Retarget() {
    _transform.LookAt(_player->GetPosition());
    _retargetTimer = RandomFloat(_retargetMin, _retargetMax);
}


void Enemy::Update(float dt) {
    if (!_alive) return;

    switch (_state) {
        case EnemyState::Moving:
            _retargetTimer -= dt;
            if (_retargetTimer <= 0.0f) Retarget();
            _transform.MoveForward(_speed * dt);
            _spriteMove.Update(dt);
            break;

        default:
            break;
        case EnemyState::Dying:
            _spriteDeath.Update(dt);
            if (_spriteDeath.finished) Deactivate();
    }
}
void Enemy::Activate(Vector2 position) {
    _alive = true;
    _transform.position = position;
    _retargetTimer = 0.0f;
    _state = EnemyState::Moving;
    _spriteDeath.Reset();
    _spriteMove.Reset();
    TraceLog(LOG_INFO,"ENEMY: Activated");
}




void Enemy::Deactivate() {
    _alive = false;
    _transform.position = GameConfig::OFFSCREEN_POSITION;
    TraceLog(LOG_INFO,"ENEMY: Deactivated");

}




void Enemy::Draw() {
    if (!_alive) return;
    switch (_state) {
        case EnemyState::Moving:
            _spriteMove.Draw(_transform);
            break;
        case EnemyState::Dying:
            _spriteDeath.Draw(_transform);
    }
    _collider.DrawDebug();

}
void Enemy::SetPosition(Vector2 position) {
    _transform.position = position;
}

void Enemy::SetPlayer(const Player* player) {
    _player = player;
}