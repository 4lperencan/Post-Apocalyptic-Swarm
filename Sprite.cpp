#include "Sprite.hpp"
#include "ResourceManager.hpp"

bool Sprite::showDebug = false;


void Sprite::Init(const std::string& textureName) {
    texture = &RM::get().GetTexture(textureName);
    frameWidth = texture->width;
    frameHeight = texture->height;
}
void Sprite::Draw(const Transform2D& transform) const {
    if (!texture) return;

    float w = frameWidth * transform.scale;
    float h = frameHeight * transform.scale;

    Rectangle source = {
        0,0, (float)frameWidth, (float)frameHeight
    };
    Rectangle dest = {
        transform.position.x,
        transform.position.y,
        w, h
    };


    Vector2 origin = {
        w * pivot.x,
        h * pivot.y
    };

    DrawTexturePro(*texture, source, dest, origin, transform.rotation, WHITE);
    if (showDebug) {
        DrawCircle((int)transform.position.x, (int)transform.position.y, 2.0f, RED);
    }
    };
