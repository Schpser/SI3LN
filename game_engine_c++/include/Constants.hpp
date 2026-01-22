#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <map>
#include <vector>

// Game constants
namespace Constants {
    // Screen settings
    constexpr int DEFAULT_SCREEN_WIDTH = 1024;
    constexpr int DEFAULT_SCREEN_HEIGHT = 768;
    constexpr int FPS = 60;
    
    // Game settings
    constexpr int MAX_LIVES = 5;
    constexpr int PLAYER_SPEED = 5;
    constexpr int ENEMY_SPEED = 2;
    constexpr int BULLET_SPEED = 7;
    constexpr int ENEMY_BULLET_SPEED = 4;
    
    // Player settings
    constexpr int PLAYER_WIDTH = 90;
    constexpr int PLAYER_HEIGHT = 90;
    constexpr int PLAYER_SHOOT_COOLDOWN = 250; // milliseconds
    
    // Enemy settings
    constexpr int ENEMY_WIDTH = 60;
    constexpr int ENEMY_HEIGHT = 60;
    constexpr int ENEMY_SHOOT_COOLDOWN = 2000; // milliseconds
    constexpr int BOSS_WIDTH = 100;
    constexpr int BOSS_HEIGHT = 100;
    constexpr int BOSS_HEALTH = 10;
    
    // Bullet settings
    constexpr int BULLET_WIDTH = 15;
    constexpr int BULLET_HEIGHT = 25;
    constexpr int ENEMY_BULLET_WIDTH = 10;
    constexpr int ENEMY_BULLET_HEIGHT = 20;
    
    // Bonus settings
    constexpr int BONUS_WIDTH = 40;
    constexpr int BONUS_HEIGHT = 40;
    constexpr int BONUS_FALL_SPEED = 3;
    constexpr int SHIELD_DURATION = 3000; // milliseconds
    constexpr int MEGA_SHOT_DURATION = 5000; // milliseconds
    constexpr int FREEZE_DURATION = 2000; // milliseconds
    
    // Special attack settings
    constexpr int SPECIAL_ATTACK_COOLDOWN = 10000; // milliseconds
    constexpr int SPECIAL_ATTACK_WIDTH = 100;
    constexpr int SPECIAL_ATTACK_HEIGHT = 100;
    
    // Score multipliers
    constexpr int SCORE_ENEMY = 100;
    constexpr int SCORE_BOSS = 1000;
    constexpr int SCORE_LEVEL_MULTIPLIER = 50;
    
    // Bonus drop chances (percentage)
    constexpr int BONUS_SHIELD_CHANCE = 10;
    constexpr int BONUS_MEGA_SHOT_CHANCE = 8;
    constexpr int BONUS_EXTRA_LIFE_CHANCE = 5;
    constexpr int BONUS_FREEZE_CHANCE = 7;
    
    // World names
    const std::vector<std::string> WORLD_NAMES = {
        "Space", "Desert", "Forest", "Marine", "Apocalyptic"
    };
    
    // Levels per world
    constexpr int LEVELS_PER_WORLD = 5;
    
    // Difficulty scaling
    constexpr float DIFFICULTY_MULTIPLIER = 1.2f;
    constexpr int BASE_ENEMY_COUNT = 8;
}

// Enums
enum class GameState {
    MAIN_MENU,
    LOGIN,
    REGISTER,
    CHARACTER_SELECT,
    LEVEL_SELECT,
    GAMEPLAY,
    PAUSE,
    GAME_OVER,
    LEVEL_WIN,
    PROFILE
};

enum class EntityType {
    PLAYER,
    ENEMY,
    BOSS,
    BULLET_PLAYER,
    BULLET_ENEMY,
    BONUS,
    SPECIAL_ATTACK
};

enum class BonusType {
    SHIELD,
    MEGA_SHOT,
    EXTRA_LIFE,
    FREEZE_ENEMIES,
    SPEED_BOOST
};

enum class EnemyType {
    BASIC,
    FAST,
    TANK,
    SHOOTER,
    BOSS
};

enum class MovementPattern {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL,
    CIRCULAR,
    ZIGZAG,
    RANDOM
};

#endif // CONSTANTS_HPP
