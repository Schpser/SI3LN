#include "../include/AssetRegistry.hpp"
#include <iostream>

AssetRegistry* AssetRegistry::instance = nullptr;

AssetRegistry::AssetRegistry() {
}

AssetRegistry* AssetRegistry::getInstance() {
    if (instance == nullptr) {
        instance = new AssetRegistry();
    }
    return instance;
}

void AssetRegistry::initialize() {
    std::cout << "Initializing Asset Registry..." << std::endl;
    
    // ===== PLAYER ASSETS =====
    // 8 player characters
    playerAssets["player_0"] = "assets/players/1000055338.png";
    playerAssets["player_1"] = "assets/players/1000055339.png";
    playerAssets["player_2"] = "assets/players/1000055340.png";
    playerAssets["player_3"] = "assets/players/1000055341.png";
    playerAssets["player_4"] = "assets/players/1000055342.png";
    playerAssets["player_5"] = "assets/players/1000055343.png";
    playerAssets["player_6"] = "assets/players/1000055344.png";
    playerAssets["player_7"] = "assets/players/1000055345.png";
    
    // ===== WORLD BACKGROUNDS =====
    worldBackgrounds["Space"] = "assets/worlds/background_space.jpg";
    worldBackgrounds["Desert"] = "assets/worlds/background_desert.png";
    worldBackgrounds["Forest"] = "assets/worlds/background_forest.png";
    worldBackgrounds["Marine"] = "assets/worlds/background_marine.jpg";
    worldBackgrounds["Apocalyptic"] = "assets/worlds/background_apocalyptic.jpg";
    worldBackgrounds["Menu"] = "assets/worlds/home_page.jpg";
    
    // ===== ENEMY ASSETS BY WORLD =====
    // Space World
    enemyAssets["Space"] = {
        "assets/enemies/Space_world/enemy.png",
        "assets/enemies/Space_world/enemy (1).png",
        "assets/enemies/Space_world/enemy (2).png",
        "assets/enemies/Space_world/enemy (3).png",
        "assets/enemies/Space_world/enemy (4).png",
        "assets/enemies/Space_world/enemy (5).png",
        "assets/enemies/Space_world/boss_enemy (1).png"  // Boss
    };
    
    // Desert World
    enemyAssets["Desert"] = {
        "assets/enemies/Desert_world/enemy (1).png",
        "assets/enemies/Desert_world/enemy (2).png",
        "assets/enemies/Desert_world/enemy (3).png",
        "assets/enemies/Desert_world/enemy (4).png",
        "assets/enemies/Desert_world/enemy (5).png",
        "assets/enemies/Desert_world/enemy (6).png",
        "assets/enemies/Desert_world/enemy (7).png",
        "assets/enemies/Desert_world/file_00000000086461f7b26b07cab866bd01.png"  // Boss
    };
    
    // Forest World
    enemyAssets["Forest"] = {
        "assets/enemies/Forest_world/enemy (1).png",
        "assets/enemies/Forest_world/enemy (2).png",
        "assets/enemies/Forest_world/enemy (3).png",
        "assets/enemies/Forest_world/enemy (4).png",
        "assets/enemies/Forest_world/enemy (5).png",
        "assets/enemies/Forest_world/enemy (6).png",
        "assets/enemies/Forest_world/enemy (7).png",
        "assets/enemies/Forest_world/boss_enemy (1).png"  // Boss
    };
    
    // Marine World
    enemyAssets["Marine"] = {
        "assets/enemies/Marine_world/enemy (1).png",
        "assets/enemies/Marine_world/enemy (2).png",
        "assets/enemies/Marine_world/enemy (3).png",
        "assets/enemies/Marine_world/enemy (4).png",
        "assets/enemies/Marine_world/enemy (5).png",
        "assets/enemies/Marine_world/enemy (6).png",
        "assets/enemies/Marine_world/enemy (7).png",
        "assets/enemies/Marine_world/enemy (8).png",
        "assets/enemies/Marine_world/boss_enemy (1).png"  // Boss
    };
    
    // Apocalyptic World
    enemyAssets["Apocalyptic"] = {
        "assets/enemies/Apocalyptic_world/enemy (1).png",
        "assets/enemies/Apocalyptic_world/enemy (2).png",
        "assets/enemies/Apocalyptic_world/enemy (3).png",
        "assets/enemies/Apocalyptic_world/enemy (4).png",
        "assets/enemies/Apocalyptic_world/enemy (5).png",
        "assets/enemies/Apocalyptic_world/enemy (6).png",
        "assets/enemies/Apocalyptic_world/enemy (7).png",
        "assets/enemies/Apocalyptic_world/enemy (8).png",
        "assets/enemies/Apocalyptic_world/boss_enemy.png"  // Boss
    };
    
    // ===== BULLET ASSETS =====
    bulletAssets["player_bullet_Space"] = "assets/sprites/player/pb_space.png";
    bulletAssets["player_bullet_Desert"] = "assets/sprites/player/pb_desert.png";
    bulletAssets["player_bullet_Forest"] = "assets/sprites/player/pb_forest.png";
    bulletAssets["player_bullet_Marine"] = "assets/sprites/player/pb_marine.png";
    bulletAssets["player_bullet_Apocalyptic"] = "assets/sprites/player/pb_apocaliptyc.png";
    
    bulletAssets["enemy_bullet_Space"] = "assets/sprites/ennemy/eb_space.png";
    bulletAssets["enemy_bullet_Desert"] = "assets/sprites/ennemy/eb_desert.png";
    bulletAssets["enemy_bullet_Forest"] = "assets/sprites/ennemy/eb_forest.png";
    bulletAssets["enemy_bullet_Marine"] = "assets/sprites/ennemy/eb_marine.png";
    bulletAssets["enemy_bullet_Apocalyptic"] = "assets/sprites/ennemy/eb_apocaliptyc.png";
    
    // Generic bullets
    bulletAssets["player_bullet"] = "assets/sprites/player/player_bullet.png";
    bulletAssets["enemy_bullet"] = "assets/sprites/ennemy/enemy_bullet.png";
    
    // ===== EXPLOSION ASSETS =====
    explosionAssets["player_explosion"] = "assets/sprites/player/pb_space.png";
    explosionAssets["enemy_explosion"] = "assets/sprites/ennemy/explosion.png";
    
    // ===== BONUS ASSETS =====
    // Note: You may need to create these or use existing sprites
    bonusAssets["shield"] = "assets/sprites/bonuses/shield.png";
    bonusAssets["mega_shot"] = "assets/sprites/bonuses/mega_shot.png";
    bonusAssets["extra_life"] = "assets/sprites/bonuses/extra_life.png";
    bonusAssets["freeze"] = "assets/sprites/bonuses/freeze.png";
    bonusAssets["speed_boost"] = "assets/sprites/bonuses/speed.png";
    
    std::cout << "Asset Registry initialized with:" << std::endl;
    std::cout << "  - " << playerAssets.size() << " player characters" << std::endl;
    std::cout << "  - " << worldBackgrounds.size() << " world backgrounds" << std::endl;
    std::cout << "  - " << enemyAssets.size() << " enemy sets" << std::endl;
    std::cout << "  - " << bulletAssets.size() << " bullet sprites" << std::endl;
}

std::string AssetRegistry::getPlayerAsset(int characterId) const {
    std::string key = "player_" + std::to_string(characterId);
    auto it = playerAssets.find(key);
    return (it != playerAssets.end()) ? it->second : "";
}

std::vector<std::string> AssetRegistry::getEnemyAssets(const std::string& world) const {
    auto it = enemyAssets.find(world);
    return (it != enemyAssets.end()) ? it->second : std::vector<std::string>();
}

std::string AssetRegistry::getEnemyAsset(const std::string& world, int index) const {
    auto assets = getEnemyAssets(world);
    if (index >= 0 && index < static_cast<int>(assets.size())) {
        return assets[index];
    }
    return "";
}

std::string AssetRegistry::getBossAsset(const std::string& world) const {
    auto assets = getEnemyAssets(world);
    // Boss is typically the last asset in the list
    return assets.empty() ? "" : assets.back();
}

std::string AssetRegistry::getWorldBackground(const std::string& world) const {
    auto it = worldBackgrounds.find(world);
    return (it != worldBackgrounds.end()) ? it->second : "";
}

std::string AssetRegistry::getPlayerBulletAsset(const std::string& world) const {
    std::string key = "player_bullet_" + world;
    auto it = bulletAssets.find(key);
    if (it != bulletAssets.end()) {
        return it->second;
    }
    // Fallback to generic
    return bulletAssets.at("player_bullet");
}

std::string AssetRegistry::getEnemyBulletAsset(const std::string& world) const {
    std::string key = "enemy_bullet_" + world;
    auto it = bulletAssets.find(key);
    if (it != bulletAssets.end()) {
        return it->second;
    }
    // Fallback to generic
    return bulletAssets.at("enemy_bullet");
}

std::string AssetRegistry::getPlayerExplosionAsset(const std::string& world) const {
    return explosionAssets.at("player_explosion");
}

std::string AssetRegistry::getEnemyExplosionAsset(const std::string& world) const {
    return explosionAssets.at("enemy_explosion");
}

std::string AssetRegistry::getBonusAsset(const std::string& bonusType) const {
    auto it = bonusAssets.find(bonusType);
    return (it != bonusAssets.end()) ? it->second : "";
}

std::vector<std::string> AssetRegistry::getAllAssetPaths() const {
    std::vector<std::string> allPaths;
    
    // Players
    for (const auto& pair : playerAssets) {
        allPaths.push_back(pair.second);
    }
    
    // Backgrounds
    for (const auto& pair : worldBackgrounds) {
        allPaths.push_back(pair.second);
    }
    
    // Enemies
    for (const auto& pair : enemyAssets) {
        for (const auto& path : pair.second) {
            allPaths.push_back(path);
        }
    }
    
    // Bullets
    for (const auto& pair : bulletAssets) {
        allPaths.push_back(pair.second);
    }
    
    // Explosions
    for (const auto& pair : explosionAssets) {
        allPaths.push_back(pair.second);
    }
    
    // Bonuses
    for (const auto& pair : bonusAssets) {
        allPaths.push_back(pair.second);
    }
    
    return allPaths;
}

bool AssetRegistry::hasPlayerAsset(int characterId) const {
    std::string key = "player_" + std::to_string(characterId);
    return playerAssets.find(key) != playerAssets.end();
}

bool AssetRegistry::hasWorldAssets(const std::string& world) const {
    return enemyAssets.find(world) != enemyAssets.end() &&
           worldBackgrounds.find(world) != worldBackgrounds.end();
}
