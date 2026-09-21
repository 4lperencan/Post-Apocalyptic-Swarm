#pragma once


#include "raylib.h"
#include "MovementState.hpp"
#include "Transform2D.hpp"
#include "Sprite.hpp"
#include "Movement.hpp"
#include "CircleCollider.hpp"

class Player {
    public:
    Player(const std::string& textureName);

    void Draw() const;
    void Update(float delta);
    void Hit();
    int GetHealth() const {return _health;}
    int GetMaxHealth() const {return _maxHealth;}

    Vector2 GetPosition() const;
    Vector2 GetFiringPosition() const;
    bool IsInvincible() const {return _invTimer > 0.0f; }

    void SetPosition(Vector2 position);
    const CircleCollider& GetCollider() const{ return _collider; };

    private:
    Transform2D _transform;
    Sprite _sprite;
    Movement _movement;
    CircleCollider _collider;
    Vector2 _muzzleOffset = {0.0f, 0.0f};
    int _maxHealth = 1;
    int _health = 1;
    float _invTimer = 0.0f;
    float _invTime = 0.0f;
};

