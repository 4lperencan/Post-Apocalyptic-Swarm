# Post Apocalyptic Swarm

A 2D top-down post-apocalyptic zombie survival shooter made with **C++17** and **raylib**. Built as a Semester 1 project for Fontys University of Applied Sciences (ICT).

Zombies come in waves and chase you. Every wave is bigger than the last one. Survive as long as you can and get the highest score.

---

## Gameplay

* Start screen, press `ENTER` to play.
* Zombies spawn in waves away from the player and chase you. When a wave is cleared the next, larger wave starts.
* Every zombie you kill gives 10 points.
* You have 3 health. After taking a hit you are invincible for 1.5 seconds (the player flashes red).
* When your health reaches 0 the game is over. Your score and wave are shown, press `R` to restart.
* Background music plays during the game, and a voice line plays when a wave is finished.

---

## Controls

| Action | Input |
| :--- | :--- |
| Start game | `ENTER` |
| Move | `W`, `A`, `S`, `D` |
| Aim | Mouse |
| Shoot | Left mouse button |
| Restart (on game over) | `R` |
| Release / lock mouse cursor | `ESC` |
| Toggle debug view (colliders) | `P` |
| Quit | `Q` |

---

## How it works

* **Game loop and states:** the game switches between `Menu`, `Playing` and `GameOver` (`GameState` in `main.cpp`). Gameplay only updates while `Playing`.
* **Object pooling:** bullets and zombies are kept in pools (`BulletManager`, `EnemyManager`, `std::vector<std::unique_ptr<...>>`). Dead objects are reused instead of being deleted and created again.
* **Enemies:** each zombie has a small state machine (`Moving`, `Dying`). It turns toward the player every 1–2 seconds, and plays a death animation when shot.
* **Collision:** circle colliders (`CircleCollider`) for bullets, zombies and the player. The arena walls use axis-separated rectangle (AABB) collision, so the player can slide along walls.
* **Aiming:** the mouse position is converted to world coordinates, so aiming is correct even when the camera stops at the edge of the map. Bullets spawn at the gun's muzzle, rotated with the player.
* **Camera:** follows the player on a 2560x1440 map and is clamped so it never shows outside the map.
* **Resolution:** the game is drawn to a fixed 1280x720 canvas and scaled to the window with letterboxing, so resizing the window keeps the same view.
* **Resources:** textures, music and sounds are loaded once by `ResourceManager` and shared.
* **Frame-rate independent movement:** all movement is multiplied by delta time.

### Project structure

| File | Purpose |
| :--- | :--- |
| `main.cpp` | Window, game loop, game states, waves, score, collisions, drawing |
| `GameConfig.hpp` | Game settings (map size, player stats, wave size, score) |
| `Player` | Player movement, health, invincibility, muzzle position |
| `Bullet`, `BulletManager` | Bullets and the bullet pool |
| `Enemy`, `EnemyManager` | Zombies, their pool and wave spawning |
| `CircleCollider` | Circle collision |
| `Sprite` | Drawing and animating sprite sheets |
| `Transform2D` | Position, rotation and scale |
| `GameInput`, `Movement`, `MovementState` | Reading input and applying it to the player |
| `ResourceManager`, `ResourceKeys` | Loading and sharing assets |
| `SwarmUtils.hpp` | Small math helpers |

---

## Building

### Requirements

* CMake 3.20 or newer
* A C++17 compiler (Clang / AppleClang / GCC)
* raylib (on macOS: `brew install raylib`)

### Build and run

```bash
git clone https://github.com/4lperencan/Post-Apocalyptic-Swarm.git
cd Post-Apocalyptic-Swarm
cmake -B build -S .
cmake --build build
./build/PostApocalypticSwarm
```

The `assets` folder is copied next to the executable automatically after building.

---

## Credits

* Music: "Cave Rave" — from itch.io, by Dani Maccari (https://dani-maccari.itch.io/jumppack-music-loops)
* Wave finished voice line — from itch.io, by Dillon Becker (https://dillonbecker.itch.io/sdap)
