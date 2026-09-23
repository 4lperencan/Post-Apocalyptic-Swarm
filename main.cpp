#include <iostream>
#include <ostream>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "algorithm"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"
#include "ResourceKeys.hpp"
#include "GameInput.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "Player.hpp"



enum class GameState {Menu, Playing, GameOver};









int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Swarm Shooter Project");
    SetTargetFPS(60);
    DisableCursor();
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    RM::get().Load();

    Music gameMusic = RM::get().GetMusic(RK::MUSIC_GAME);
    Sound waveFinishedSound = RM::get().GetSound(RK::SFX_WAVE_FINISHED);
    SetMusicVolume(gameMusic, 0.5f);

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

    int wave = 1;
    int score = 0;

    EnemyManager enemies;
    enemies.Init(&player);
    enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);



    Vector2 pPos = player.GetPosition();
    float playerSize = 32.0f;
    Rectangle playerHitbox = {
        pPos.x - playerSize * 0.5f,
        pPos.y - playerSize * 0.5f,
        playerSize,
        playerSize
    };

    GameState gameState = GameState::Menu;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P)) GameConfig::SHOW_DEBUG = !GameConfig::SHOW_DEBUG;
        if (IsKeyPressed(KEY_Q))
            break;
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        UpdateMusicStream(gameMusic);

        if (gameState == GameState::Playing && enemies.IsBatchComplete()) {
            wave++;
            PlaySound(waveFinishedSound);
            enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);

        }

        if (gameState == GameState::Menu && IsKeyPressed(KEY_ENTER)) {
            gameState = GameState::Playing;
            PlayMusicStream(gameMusic);
        }

        if (gameState == GameState::GameOver && IsKeyPressed(KEY_R)) {
            player.Reset();
            player.SetPosition({GameConfig::MAP_W * 0.5f, GameConfig::MAP_H * 0.5f});
            bullets.DeactivateAll();
            enemies.DeactivateAll();
            gameState = GameState::Playing;
            wave = 1;
            score = 0;
            enemies.SpawnBatch(GameConfig::WAVE_ENEMY_BASE + GameConfig::WAVE_ENEMY_RAMP * wave);
            PlayMusicStream(gameMusic);


        }



        float scale = std::min(
            float(GetScreenWidth() / (float)screenWidth),
            float(GetScreenHeight() / (float)screenHeight)
            );
        float offsetX = (GetScreenWidth() - screenWidth * scale) * 0.5f;
        float offsetY = (GetScreenHeight() - screenHeight * scale) * 0.5f;
        SetMouseOffset((int)-offsetX, (int)-offsetY);
        SetMouseScale(1.0f / scale, 1.0f / scale);

        GI::get().Update(camera, player.GetPosition());

        float dt = GetFrameTime();

        if (gameState == GameState::Playing) {
            Vector2 oldPos = player.GetPosition();
            player.Update(dt);
            Vector2 newPos = player.GetPosition();

            auto hitsWall = [&](Vector2 center) {
                Rectangle box = { center.x - playerSize * 0.5f, center.y - playerSize * 0.5f, playerSize, playerSize };
                for (const auto& wall : wallColliders) {
                    if (CheckCollisionRecs(box, wall)) return true;
                }
                return false;
            };

            Vector2 resolved = oldPos;
            resolved.x = newPos.x;
            if (hitsWall(resolved)) resolved.x = oldPos.x;
            resolved.y = newPos.y;
            if (hitsWall(resolved)) resolved.y = oldPos.y;
            player.SetPosition(resolved);

            if (GI::get().State().shoot) {
               bullets.Spawn(player.GetFiringPosition(),GI::get().State().aimAngle);
            }
            bullets.Update(dt);
            enemies.Update(dt);

            for (auto& bullet : bullets.GetPool()) {
                if (!bullet->IsAlive()) continue;
                for (auto& enemy : enemies.GetPool()) {
                    if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;
                    if (bullet->GetCollider().IsCollidingWith(enemy->GetCollider())) {
                        TraceLog(LOG_INFO,"HIT!!!");
                        bullet->Deactivate();
                        enemy->Kill();
                        score += GameConfig::SCORE_PER_KILL;
                        break;
                    }
                }

            }
            for (auto& enemy : enemies.GetPool()) {
                if (!enemy->IsAlive() || !enemy->CanBeHit()) continue;
                if (player.GetCollider().IsCollidingWith(enemy->GetCollider())) {
                    player.Hit();
                }

            }

            camera.target = player.GetPosition();

            camera.target.x = std::clamp(camera.target.x, halfW, GameConfig::MAP_W - halfW);
            camera.target.y = std::clamp(camera.target.y, halfH, GameConfig::MAP_H - halfH);

        if (player.IsDead()) {
            gameState = GameState::GameOver;
            StopMusicStream(gameMusic);
        }

        }

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
        enemies.Draw();

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
        if (gameState != GameState::Menu) {
            DrawRectangle(0, screenHeight - 32, screenWidth, 32, ColorAlpha(DARKBLUE, 0.6f));
            DrawText(TextFormat("Player: %.0f, %.0f", player.GetPosition().x, player.GetPosition().y), 12, screenHeight - 24, 20, LIME);
            DrawText(TextFormat("Camera: %.0f, %.0f", camera.target.x, camera.target.y), 256, screenHeight - 24, 20, LIME);
            DrawText(TextFormat("Aim: %.1f", GI::get().State().aimAngle), 512, screenHeight - 24, 20, LIME);


            DrawText(TextFormat("WV:%d  HP: %d/%d  Bullets: %d/%d Enemies: %d/%d",
            wave,
            player.GetHealth(), player.GetMaxHealth(),
                (int)bullets.CountAlive(), bullets.GetPoolTotal(),enemies.CountAlive(), enemies.GetPoolTotal()),
                700, screenHeight - 24, 20, LIME);

            const char* scoreText = TextFormat("SCORE: %d", score);
            int scoreW = MeasureText(scoreText, 32);
            DrawText(scoreText, GameConfig::BASE_W - scoreW - 20, 16, 32, RAYWHITE);
        }

        if (gameState == GameState::Menu) {
            DrawRectangle(0, 0, GameConfig::BASE_W, GameConfig::BASE_H, ColorAlpha(BLACK, 0.75f));

            const char* menuTitle = "SWARM SHOOTER";
            int menuTitleW = MeasureText(menuTitle, 72);
            DrawText(menuTitle, (GameConfig::BASE_W - menuTitleW) / 2,
                GameConfig::BASE_H / 2 - 120,
                72, LIME);

            const char* startPrompt = "Press ENTER to start";
            int startPromptW = MeasureText(startPrompt, 32);
            DrawText(startPrompt, (GameConfig::BASE_W - startPromptW) / 2,
                GameConfig::BASE_H / 2,
                32, RAYWHITE);

            const char* controls = "WASD: move    Mouse: aim    Left click: shoot";
            int controlsW = MeasureText(controls, 20);
            DrawText(controls, (GameConfig::BASE_W - controlsW) / 2,
                GameConfig::BASE_H / 2 + 60,
                20, LIGHTGRAY);
        }


        if (gameState == GameState::GameOver) {
            DrawRectangle(0,0, GameConfig::BASE_W,
                GameConfig::BASE_H, ColorAlpha(BLACK,
                    0.7f));
            const char* title  = "GAME OVER";
            int titleW = MeasureText(title, 60);
            DrawText(title, (GameConfig::BASE_W - titleW) / 2,
                GameConfig::BASE_H / 2 - 60,
                60, RED);

            const char* finalScore = TextFormat("Score: %d   Wave: %d", score, wave);
            int finalScoreW = MeasureText(finalScore, 32);
            DrawText(finalScore, (GameConfig::BASE_W - finalScoreW) / 2,
                GameConfig::BASE_H / 2 + 12,
                32, YELLOW);

            const char* prompt = "Press R to restart";
            int promptW = MeasureText(prompt, 32);
            DrawText(prompt, (GameConfig::BASE_W - promptW) / 2,
                GameConfig::BASE_H / 2 + 60,
                32, RAYWHITE);
        }

        EndTextureMode();

        Rectangle src = { 0, 0, (float)screenWidth, -(float)screenHeight };
        Rectangle dest = {offsetX, offsetY, screenWidth * scale, screenHeight * scale};

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas.texture, src, dest, {0,0}, 0.0f, WHITE);


        EndDrawing();
    }
    UnloadRenderTexture(canvas);
    RM::get().Unload();
    CloseAudioDevice();

    CloseWindow();
    return 0;
}
