#include "EnemyManager.hpp"

#include "GameConfig.hpp"
#include "Player.hpp"
#include "raylib.h"
#include <raymath.h>
#include "SwarmUtils.hpp"

void EnemyManager::Init(Player *player) {
    _player = player;
}

Vector2 EnemyManager::pickSpawnPoint() const {
    constexpr float MIN_DEST = 200.0f;
    constexpr float MIN_DEST_SQ = MIN_DEST * MIN_DEST;
    Vector2 playerPos = _player->GetPosition();
    Vector2 candidate;

    do {
        candidate.x = RandomFloat(0.0f, GameConfig::MAP_W);
        candidate.y = RandomFloat(0.0f, GameConfig::MAP_H);
    } while (Vector2DistanceSqr(candidate, playerPos) < MIN_DEST_SQ);


    return candidate;

}




void EnemyManager::SpawnBatch(int count) {
    _batchRemaining = count;
    _staggerTimer = 0.0f;
    TraceLog(LOG_INFO,"ENEMY_MGR: Batch of %d enemies queued", count);

}



void EnemyManager::Spawn(Vector2 pos) {
    for (auto& enemy : _pool) {
        if (!enemy->IsAlive()) {
            enemy->Activate(pos);
            return;
        }
    }
    auto enemy = std::make_unique<Enemy>();
    enemy->SetPlayer(_player);
    enemy->Activate(pos);
    _pool.push_back(std::move(enemy));
}

void EnemyManager::Update(float dt) {
    if (_batchRemaining > 0) {
        _staggerTimer -= dt;
        if (_staggerTimer <= 0.0f) {
            _staggerTimer = _staggerInterval;
            _batchRemaining--;
            Spawn(pickSpawnPoint());
        }
    }
    for (auto& enemy : _pool) enemy->Update(dt);

}
void EnemyManager::Draw() {
    for (auto& enemy : _pool) enemy->Draw();

}
void EnemyManager::DeactivateAll() {
    for (auto& enemy : _pool) enemy->Deactivate();

}