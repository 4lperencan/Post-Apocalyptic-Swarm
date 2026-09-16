#pragma once
#include "string"
#include <raylib.h>
#include "Transform2D.hpp"


struct Sprite {
    static bool showDebug;
    const Texture2D* texture = nullptr;
    Vector2 pivot = { 0.5f, 0.5f };
    int frameWidth = 0;
    int frameHeight = 0;


    void Init(const std::string& textureName);
    void Draw(const Transform2D& transform) const;
};