#include "ResourceManager.hpp"
#include "ResourceKeys.hpp"
#include "stdexcept"

void ResourceManager::Unload() {
    for (auto& [name, tex] : _textures)
        UnloadTexture(tex);
    _textures.clear();


    for (auto& [name, img] : _images)
        UnloadImage(img);
    _images.clear();
    TraceLog(LOG_INFO, "ResourceManager: unloaded");
}

void ResourceManager::Load() {
    loadTextures(RK::GAME_BG, "assets/floor.png");
    loadTextures(RK::GAME_FG, "assets/walls.png");
    loadTextures(RK::PLAYER,  "assets/player_9mmhandgun.png");
    loadTextures(RK::BULLET, "assets/bullet.png");
    loadTextures(RK::ZOMBIE_MOVE, "assets/zombiewalk.png");
    loadTextures(RK::ZOMBIE_DEATH,  "assets/zombiedeath.png");



    TraceLog(LOG_INFO, "ResourceManager: loaded %d textures", (int)_textures.size());
    TraceLog(LOG_INFO, "ResourceManager: loaded %d images", (int)_images.size());
}
 void ResourceManager::loadTextures(const std::string& name, const std::string& path) {
     Texture2D tex = LoadTexture(path.c_str());
     if (tex.id == 0)
        throw std::runtime_error("LoadTexture failed: " + path);
     _textures.emplace(name, std::move(tex));
 }
void ResourceManager::loadImage(const std::string& name, const std::string& path) {
    Image img = LoadImage(path.c_str());
    if (img.data == nullptr)
        throw std::runtime_error("Failed to load image " + path);
    _images.emplace(name, std::move(img));
}




const Texture2D& ResourceManager::GetTexture(const std::string& name) const {
     auto it = _textures.find(name);
     if (it == _textures.end())
         throw std::runtime_error("Texture not found: " + name + "'");
     return it->second;
 }

const Image& ResourceManager::GetImage(const std::string& name) const {
    auto it = _images.find(name);
    if (it == _images.end())
        throw std::runtime_error("Image not found: " + name + "'");
    return it->second;
}