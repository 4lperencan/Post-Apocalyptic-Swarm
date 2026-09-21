#pragma once
#include "CircleCollider.hpp"
#include "Transform2D.hpp"
#include "Sprite.hpp"


class Player;


class Enemy {

public:
    Enemy();
    void Update(float dt);
    void Draw();
    void SetPosition(Vector2 position);
    void SetPlayer(const Player *player);
    void Deactivate();
    void Activate(Vector2 pos);
    bool IsAlive() const { return _alive; }
    const CircleCollider& GetCollider() const { return _collider; }



private:
    Transform2D _transform;
    Sprite _sprite;
    CircleCollider _collider;
    const Player* _player = nullptr;
    float _speed = 80.0f;
    float _retargetTimer = 0.0f;
    float _retargetMin = 1.0f;
    float _retargetMax = 2.0f;

    bool _alive = true;


    void Retarget();


};