#ifndef ASSET_REGISTRY_HPP
#define ASSET_REGISTRY_HPP

#include <string>
#include <map>
#include <vector>

/**
 * Asset Registry
 * 
 * This class manages asset paths and IDs.
 * The C++ engine does NOT load PNG/JPG files directly.
 * Instead, it provides asset IDs that the frontend (HTML/CSS/JS) uses to load the actual images.
 * 
 * The frontend is responsible for:
 * - Loading image files
 * - Rendering sprites at positions provided by C++
 * - Displaying UI elements
 */

class AssetRegistry {
private:
    static AssetRegistry* instance;
    
    // Asset path mappings
    std::map<std::string, std::string> playerAssets;
    std::map<std::string, std::vector<std::string>> enemyAssets;
    std::map<std::string, std::string> worldBackgrounds;
    std::map<std::string, std::string> bulletAssets;
    std::map<std::string, std::string> explosionAssets;
    std::map<std::string, std::string> bonusAssets;
    
    AssetRegistry();
    
public:
    static AssetRegistry* getInstance();
    ~AssetRegistry() = default;
    
    // Initialize asset registry
    void initialize();
    
    // Player assets
    std::string getPlayerAsset(int characterId) const;
    int getPlayerCount() const { return playerAssets.size(); }
    
    // Enemy assets
    std::vector<std::string> getEnemyAssets(const std::string& world) const;
    std::string getEnemyAsset(const std::string& world, int index) const;
    std::string getBossAsset(const std::string& world) const;
    
    // World backgrounds
    std::string getWorldBackground(const std::string& world) const;
    
    // Bullet assets
    std::string getPlayerBulletAsset(const std::string& world) const;
    std::string getEnemyBulletAsset(const std::string& world) const;
    
    // Explosion assets
    std::string getPlayerExplosionAsset(const std::string& world) const;
    std::string getEnemyExplosionAsset(const std::string& world) const;
    
    // Bonus assets
    std::string getBonusAsset(const std::string& bonusType) const;
    
    // Get all asset paths (for preloading in frontend)
    std::vector<std::string> getAllAssetPaths() const;
    
    // Asset existence checking
    bool hasPlayerAsset(int characterId) const;
    bool hasWorldAssets(const std::string& world) const;
};

#endif // ASSET_REGISTRY_HPP
