#include <iostream>
#include <ostream>
#include <vector>
#include "raylib.h"
#include "algorithm"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"
#include "ResourceKeys.hpp"
#include "GameInput.hpp"
#include "BulletManager.hpp"
#include "Player.hpp"
#include "raymath.h"




int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Swarm Shooter Project");
    SetTargetFPS(60);
    DisableCursor();
    RM::get().Load();

    const Texture2D& background = RM::get().GetTexture(RK::GAME_BG);
    GameConfig::MAP_W = 2560.0f;
    GameConfig::MAP_H = 1440.0f;

    const float tileSize = 64.0f;
    Rectangle wallSourceRec = {0.0f, 0.0f, 16.0f, 16.0f};
    Vector2 origin = { 0.0f, 0.0f };
    RenderTexture2D canvas = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);
    Rectangle playerSrc = {0.0f,0.0f,(float)RM::get().GetTexture(RK::PLAYER).width,(float)RM::get().GetTexture(RK::PLAYER).height};

    float halfW = screenWidth * 0.5f;
    float halfH = screenHeight * 0.5f;

    std::vector<Rectangle> wallColliders;
    for (int x = 0; x < (int)GameConfig::MAP_W; x += (int)tileSize) {
        wallColliders.push_back({(float)x, 0.0f, tileSize, tileSize});
        wallColliders.push_back({(float)x, GameConfig::MAP_H- tileSize, tileSize, tileSize});
    }
    for (int y = 0; y < (int)GameConfig::MAP_H; y += (int)tileSize) {
        wallColliders.push_back({ 0.0f, (float)y, tileSize, tileSize });
        wallColliders.push_back({ GameConfig::MAP_W - tileSize, (float)y, tileSize, tileSize });
}



   Player player(RK::PLAYER);
    player.SetPosition({GameConfig::MAP_W * 0.5f, GameConfig::MAP_H * 0.5f});
    Camera2D camera = {};
    camera.zoom = 1.0f;
    camera.target = player.GetPosition();
    camera.offset = {halfW,halfH};

    BulletManager bullets;

    Vector2 pPos = player.GetPosition();
    float playerSize = 32.0f;
    Rectangle playerHitbox = {
        pPos.x - playerSize * 0.5f,
        pPos.y - playerSize * 0.5f,
        playerSize,
        playerSize
    };

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P)) Sprite::showDebug = !Sprite::showDebug;

        GI::get().Update();
        Vector2 moveDir = GI::get().State().moveDir;


        float dt = GetFrameTime();

        if (GI::get().State().shoot) {
           bullets.Spawn(player.GetFiringPosition(),GI::get().State().aimAngle);
        }
        player.Update(dt);
        bullets.Update(dt);








        Vector2 oldPos = player.GetPosition();
        playerSize = 32.0f;

        player.SetPosition({ oldPos.x + moveDir.x * 200.0f * dt, oldPos.y});
        Rectangle boxX = { player.GetPosition().x - playerSize * 0.5f, player.GetPosition().y - playerSize * 0.5f, playerSize, playerSize };

        for (const auto& wall : wallColliders) {
    if (CheckCollisionRecs(boxX, wall)) {
        player.SetPosition({ oldPos.x, player.GetPosition().y });
        break;
    }
}
    Vector2 currentPos = player.GetPosition();
player.SetPosition({ currentPos.x, currentPos.y + moveDir.y * 200.0f * dt });
Rectangle boxY = { player.GetPosition().x - playerSize * 0.5f, player.GetPosition().y - playerSize * 0.5f, playerSize, playerSize };

for (const auto& wall : wallColliders) {
    if (CheckCollisionRecs(boxY, wall)) {
        player.SetPosition({ player.GetPosition().x, currentPos.y });
        break;
    }
}


        camera.target = player.GetPosition();



        camera.target.x = std::clamp(camera.target.x, halfW, GameConfig::MAP_W - halfW);
        camera.target.y = std::clamp(camera.target.y, halfH, GameConfig::MAP_H - halfH);
        player.Update(dt);
        BeginTextureMode(canvas);
        ClearBackground(BLACK);
        BeginMode2D(camera);



        for (int y = 0; y < 0; y += background.height) {
            for (int x = 0; x < screenWidth; x += background.width) {
                DrawTexture(background, x, y, WHITE);
            }
        }
        for (int y = 0; y < (int)GameConfig::MAP_H; y += background.height) {
            for (int x = 0; x < (int)GameConfig::MAP_W; x += background.width) {
                DrawTexture(background, x, y, WHITE);
            }
        }

        player.Draw();


        bullets.Draw();


        for (int x = 0; x < (int)GameConfig::MAP_W; x += (int)tileSize) {
            Rectangle topDest = { (float)x, 0.0f, tileSize, tileSize };
            DrawTexturePro(RM::get().GetTexture(RK::GAME_FG), wallSourceRec, topDest, origin, 0.0f, WHITE);

            Rectangle bottomDest = { (float)x, GameConfig::MAP_H - tileSize, tileSize, tileSize };
            DrawTexturePro(RM::get().GetTexture(RK::GAME_FG), wallSourceRec, bottomDest, origin, 0.0f, WHITE);
        }

        for (int y = 0; y < (int)GameConfig::MAP_H; y += (int)tileSize) {
            Rectangle leftDest = { 0.0f, (float)y, tileSize, tileSize };
            DrawTexturePro(RM::get().GetTexture(RK::GAME_FG), wallSourceRec, leftDest, origin, 0.0f, WHITE);

            Rectangle rightDest = { GameConfig::MAP_W - tileSize, (float)y, tileSize, tileSize };
            DrawTexturePro(RM::get().GetTexture(RK::GAME_FG), wallSourceRec, rightDest, origin, 0.0f, WHITE);
        }
        EndMode2D();
        DrawRectangle(0, screenHeight - 32, screenWidth, 32, ColorAlpha(DARKBLUE, 0.6f));
        DrawText(TextFormat("Player: %.0f, %.0f", player.GetPosition().x, player.GetPosition().y), 12, screenHeight - 24, 20, LIME);
        DrawText(TextFormat("Camera: %.0f, %.0f", camera.target.x, camera.target.y), 256, screenHeight - 24, 20, LIME);
        DrawText(TextFormat("Aim: %1.f", GI::get().State().aimAngle), 512, screenHeight - 24, 20, LIME);
        DrawText(TextFormat("Bullets: %d/%d", (int)bullets.CountAlive(), bullets.GetPoolTotal()), 700, screenHeight - 24, 20, LIME);


        EndTextureMode();
        float scale = std::min(
            float(GetScreenWidth() / (float)screenWidth),
            float(GetScreenHeight() / (float)screenHeight)
            );

        float offsetX = (GetScreenWidth() - screenWidth * scale) * 0.5f;
        float offsetY = (GetScreenHeight() - screenHeight * scale) * 0.5f;

        Rectangle src = { 0, 0, (float)screenWidth, -(float)screenHeight };
        Rectangle dest = {offsetX, offsetY, screenWidth * scale, screenHeight * scale};

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas.texture, src, dest, {0,0}, 0.0f, WHITE);


        EndDrawing();
    }
    UnloadRenderTexture(canvas);
    RM::get().Unload();

    CloseWindow();
    return 0;
}
