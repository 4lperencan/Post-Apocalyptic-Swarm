#pragma once
#include "Transform2D.hpp"
#include "Sprite.hpp"


class Enemy {

public:
    Enemy();
    void Update(float dt);
    void Draw();
    void SetPosition(Vector2 position);


private:
    Transform2D _transform;
    Sprite _sprite;
    float _speed = 80.0f;

};