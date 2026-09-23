cmake_minimum_required(VERSION 3.20)
project(PostApocalypticSwarm)

set(CMAKE_CXX_STANDARD 17)

# Apple Silicon Homebrew dizinini tanıt
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

# Raylib paketini bul
find_package(raylib REQUIRED)

# Kaynak dosyan src/main.cpp olduğu için:
add_executable(PostApocalypticSwarm main.cpp Sprite.cpp
        GameConfig.hpp
        Player.cpp
        Movement.cpp
        GameInput.cpp
        ResourceManager.cpp
        Bullet.cpp
        BulletManager.cpp
        Enemy.cpp
        Enemy.hpp
        EnemyManager.cpp
        EnemyManager.hpp
        CircleCollider.cpp
)

# Raylib ve macOS framework'lerini bağla
target_link_libraries(PostApocalypticSwarm PRIVATE
        raylib
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo"
)

# Assets klasörünü build çıktısına otomatik kopyala (resim yolları patlamasın)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET PostApocalypticSwarm POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:PostApocalypticSwarm>/assets
    )
endif()
