#pragma once

#include <string>
#include <map>
#include <SDL2/SDL.h>

namespace SI3LN {

// Screen settings
constexpr int DEFAULT_SCREEN_WIDTH = 1280;
constexpr int DEFAULT_SCREEN_HEIGHT = 720;
constexpr int FPS = 60;
constexpr int FRAME_DELAY = 1000 / FPS;

// Colors (RGBA format)
struct Color {
    uint8_t r, g, b, a;
    
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
    
    SDL_Color toSDL() const { return {r, g, b, a}; }
};

namespace Colors {
    const Color WHITE(255, 255, 255);
    const Color BLACK(0, 0, 0);
    const Color RED(255, 0, 0);
    const Color GREEN(0, 255, 0);
    const Color BLUE(0, 0, 255);
    const Color YELLOW(255, 255, 0);
    const Color CYAN(0, 255, 255);
    const Color MAGENTA(255, 0, 255);
    const Color GRAY(128, 128, 128);
    const Color LIGHT_GRAY(200, 200, 200);
    const Color DARK_GRAY(50, 50, 50);
    const Color ORANGE(255, 165, 0);
    const Color PURPLE(128, 0, 128);
}

// Game settings
constexpr int MAX_LIVES = 5;
constexpr int MAX_PLAYER_BULLETS = 3;
constexpr int PLAYER_SPEED = 16; // Augmenter la vitesse de base du joueur
constexpr int ENEMY_SPEED = 1;
constexpr int PLAYER_BULLET_SPEED = 10;
constexpr int ENEMY_BULLET_SPEED = 5;

// Timers and durations (in milliseconds)
constexpr int SHIELD_DURATION = 3000;
constexpr int MEGA_SHOT_DURATION = 5000;
constexpr int SPECIAL_ATTACK_COOLDOWN = 10000;

// Font sizes
constexpr int FONT_SIZE_LARGE = 70;
constexpr int FONT_SIZE_MEDIUM = 40;
constexpr int FONT_SIZE_SMALL = 28;
constexpr int FONT_SIZE_TINY = 20;

// Sprite sizes
constexpr int PLAYER_PORTRAIT_SIZE = 60;
constexpr int ENEMY_SIZE = 60;
constexpr int BOSS_SIZE = 100;

// Enemy spawn parameters
constexpr int ENEMY_SPAWN_BASE_ROWS = 3;
constexpr int ENEMY_SPAWN_BASE_COLS = 5;
constexpr int ENEMY_SPAWN_MAX_ROWS = 6;
constexpr int ENEMY_SPAWN_MAX_COLS = 9;
constexpr int ENEMY_SPACING_X = 20;
constexpr int ENEMY_SPACING_Y = 20;

// Probabilities
constexpr float BONUS_DROP_CHANCE = 0.2f;
constexpr float ENEMY_SHOOT_CHANCE_BASE = 0.1f;

// Game states
enum class GameState {
    MAIN_MENU,
    LOGIN,
    REGISTER,
    PLAYER_SELECT,
    LEVEL_SELECT,
    GAMEPLAY,
    PAUSE,
    LEVEL_WIN,
    GAME_OVER,
    PROFILE,
    HELP
};

// World configuration
struct WorldConfig {
    std::string name;
    std::string background;
    int levels;
    std::string enemiesDir;
    int enemyCount;
    std::pair<Color, Color> playerBulletColors;
    std::pair<Color, Color> enemyBulletColors;
};

// Worlds data
inline std::map<std::string, WorldConfig> getWorldsConfig() {
    std::map<std::string, WorldConfig> worlds;
    
    worlds["Space"] = {
        "Space World",
        "background_space.jpg",
        5,
        "Space_world",
        15,
        {Color(0, 150, 255), Color(100, 200, 255)},
        {Color(255, 50, 150), Color(255, 150, 200)}
    };
    
    worlds["Desert"] = {
        "Desert World",
        "background_desert.png",
        5,
        "Desert_world",
        8,
        {Color(255, 200, 0), Color(255, 255, 100)},
        {Color(200, 100, 0), Color(255, 150, 50)}
    };
    
    worlds["Forest"] = {
        "Forest World",
        "background_forest.png",
        5,
        "Forest_world",
        9,
        {Color(50, 255, 150), Color(150, 255, 200)},
        {Color(150, 50, 200), Color(200, 100, 255)}
    };
    
    worlds["Marine"] = {
        "Marine World",
        "background_marine.jpg",
        5,
        "Marine_world",
        12,
        {Color(0, 255, 255), Color(150, 255, 255)},
        {Color(255, 100, 0), Color(255, 200, 100)}
    };
    
    worlds["Apocalyptic"] = {
        "Apocalyptic World",
        "background_apocalyptic.jpg",
        5,
        "Apocalyptic_world",
        9,
        {Color(255, 50, 50), Color(255, 150, 150)},
        {Color(0, 255, 0), Color(150, 255, 150)}
    };
    
    return worlds;
}

} // namespace SI3LN
