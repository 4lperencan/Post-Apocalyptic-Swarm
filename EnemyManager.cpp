#include "EnemyManager.hpp"
#include "Raylib.h"

void EnemyManager::Init(Player *player) {
    _player = player;
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
    for (auto& enemy : _pool) enemy->Update(dt);

}
void EnemyManager::Draw() {
    for (auto& enemy : _pool) enemy->Draw();

}
void EnemyManager::DeactivateAll() {
    for (auto& enemy : _pool) enemy->Deactivate();

}