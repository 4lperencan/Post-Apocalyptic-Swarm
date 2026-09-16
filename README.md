# SwarmShooter

# SwarmShooter

A 2D top-down wave-based arena survival game developed with **C++17** and the **Raylib** library. This project is built as a Semester 1 foundation project for Fontys University of Applied Sciences (ICT).

---

## Features & Implementation Highlights

* **Object Pooling Pattern:** Projectiles are managed through a custom `BulletManager` pool (`std::vector<std::unique_ptr<Bullet>>`) to prevent runtime memory allocations and heap fragmentation during heavy combat.
* **Canvas-Based Letterboxing:** Game rendering targets an internal `1280x720` render texture (`RenderTexture2D`) with bilinear filtering, preserving native aspect ratios across resizable windows with letterbox/pillarbox padding.
* **Camera Clamping & Dynamic Arena:** A 2D camera smoothly tracks the player across a `2560x1440` world boundary with hard coordinate clamping to eliminate off-map rendering artifacts.
* **Muzzle Vector Math:** Accurate projectile spawn origins calculated via weapon pivot offset vectors rotated by player aim angle (`DEG2RAD` rotational transforms).
* **Grid & AABB Collision:** Tile-based arena boundary colliders (`64x64`) with axis-separated Axis-Aligned Bounding Box (AABB) resolution preventing player penetration through walls.

---

## Controls

| Action | Input |
| :--- | :--- |
| **Move** | `W`, `A`, `S`, `D` |
| **Aim** | Mouse Position |
| **Shoot** | Left Mouse Button |
| **Toggle Hitbox Debug** | `P` |

---

## Tech Stack & Architecture

* **Language:** C++17
* **Framework:** Raylib (Hardware Accelerated 2D/3D)
* **Build System:** CMake & Ninja
* **Core Architecture:**
    * `Player`: Transform management, velocity updates, and directional muzzle offsets.
    * `Bullet` & `BulletManager`: Reusable projectile instances with activation/deactivation life cycles.
    * `GameInput`: Centralized input polling and normalized mouse-angle calculations.
    * `ResourceManager`: Asset loading and cached texture retrieval.
    * `GameConfig`: Centralized constant configurations for map, arena bounds, and player attributes.

---

## Getting Started

### Prerequisites

* CMake (>= 3.20)
* C++17 compatible compiler (Clang / AppleClang / GCC)
* Raylib installed via Homebrew (`brew install raylib`) or linked as a system library

### Building

```bash
# Clone the repository
git clone [https://github.com/4lperencan/SwarmShooter.git](https://github.com/4lperencan/SwarmShooter.git)
cd SwarmShooter

# Generate build files and compile
cmake -B build -S .
cmake --build build

# Run the executable
./build/SwarmShooter
