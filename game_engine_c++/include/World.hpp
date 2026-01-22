#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <vector>
#include <map>
#include "Constants.hpp"

struct WorldTheme {
    std::string name;
    std::string backgroundTexture;
    std::string playerBulletColor;
    std::string enemyBulletColor;
    std::vector<std::string> enemyTextures;
    std::string bossTexture;
    std::string musicTrack;
    float difficultyMultiplier;
};

class World {
private:
    std::string name;
    int totalLevels;
    WorldTheme theme;
    std::vector<int> unlockedLevels;
    int currentLevel;
    
public:
    World(const std::string& name, const WorldTheme& theme, int totalLevels = Constants::LEVELS_PER_WORLD);
    ~World() = default;
    
    // Level management
    bool isLevelUnlocked(int level) const;
    void unlockLevel(int level);
    void unlockNextLevel();
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    
    // Getters
    std::string getName() const { return name; }
    int getTotalLevels() const { return totalLevels; }
    WorldTheme getTheme() const { return theme; }
    std::vector<int> getUnlockedLevels() const { return unlockedLevels; }
    
    // Difficulty calculation based on level
    float getDifficultyMultiplier(int level) const;
    int getEnemyCount(int level) const;
    bool hasBoss(int level) const;
};

#endif // WORLD_HPP
