#include "../include/World.hpp"
#include <algorithm>

World::World(const std::string& name, const WorldTheme& theme, int totalLevels)
    : name(name), totalLevels(totalLevels), theme(theme), currentLevel(1) {
    // Unlock first level by default
    unlockedLevels.push_back(1);
}

bool World::isLevelUnlocked(int level) const {
    return std::find(unlockedLevels.begin(), unlockedLevels.end(), level) != unlockedLevels.end();
}

void World::unlockLevel(int level) {
    if (level <= totalLevels && !isLevelUnlocked(level)) {
        unlockedLevels.push_back(level);
        std::sort(unlockedLevels.begin(), unlockedLevels.end());
    }
}

void World::unlockNextLevel() {
    int nextLevel = currentLevel + 1;
    if (nextLevel <= totalLevels) {
        unlockLevel(nextLevel);
    }
}

float World::getDifficultyMultiplier(int level) const {
    return theme.difficultyMultiplier * (1.0f + (level - 1) * (Constants::DIFFICULTY_MULTIPLIER - 1.0f));
}

int World::getEnemyCount(int level) const {
    int baseCount = Constants::BASE_ENEMY_COUNT;
    float multiplier = getDifficultyMultiplier(level);
    return static_cast<int>(baseCount * multiplier);
}

bool World::hasBoss(int level) const {
    // Boss appears every 5th level
    return (level % 5 == 0);
}
