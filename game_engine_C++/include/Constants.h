#pragma once

#include <string>
#include <map>
#include <SDL2/SDL.h>

namespace SI3LN {

// Paramètres d'écran
constexpr int DEFAULT_SCREEN_WIDTH = 1280;
constexpr int DEFAULT_SCREEN_HEIGHT = 720;
constexpr int FPS = 60; // Le jeu tourne à 60 images par seconde (FPS)
constexpr int FRAME_DELAY = 1000 / FPS; // Délai entre chaque image pour maintenir la vitesse du jeu constante

// Couleurs en format RGBA
struct Color {
    uint8_t r, g, b, a;
    
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
    
    SDL_Color toSDL() const { return {r, g, b, a}; } // Convertir la couleur en format SDL
};

namespace Colors {
    const Color WHITE = {255, 255, 255, 255}; // Blanc
    const Color BLACK = {0, 0, 0, 255}; // Noir
    const Color RED = {255, 0, 0, 255}; // Rouge
    const Color GREEN = {0, 255, 0, 255}; // Vert
    const Color BLUE = {0, 0, 255, 255}; // Bleu
    const Color YELLOW = {255, 255, 0, 255}; // Jaune
    const Color CYAN = {0, 255, 255, 255}; // Cyan
    const Color MAGENTA = {255, 0, 255, 255}; // Magenta
    const Color GRAY = {128, 128, 128, 255}; // Gris
    const Color LIGHT_GRAY = {200, 200, 200, 255}; // Gris clair
    const Color DARK_GRAY = {50, 50, 50, 255}; // Gris foncé
    const Color ORANGE = {255, 165, 0, 255}; // Orange
    const Color PURPLE = {128, 0, 128, 255}; // Violet
}

// Paramètres du jeu
constexpr int MAX_LIVES = 5; // Nombre maximum de vies
constexpr int MAX_PLAYER_BULLETS = 3; // Nombre maximum de balles joueur
constexpr int PLAYER_SPEED = 500; // Vitesse du joueur (non modifiée par le commentaire)
constexpr int ENEMY_SPEED = 1; // Vitesse initiale des ennemis
constexpr int PLAYER_BULLET_SPEED = 10; // Vitesse des balles du joueur
constexpr int ENEMY_BULLET_SPEED = 5; // Vitesse des balles des ennemis

// Durées et délais (en millisecondes)
constexpr int SHIELD_DURATION = 3000; // Durée de protection du bouclier
constexpr int MEGA_SHOT_DURATION = 5000; // Durée du tir méga
constexpr int SPECIAL_ATTACK_COOLDOWN = 10000; // Délai entre les attaques spéciales

// Taille des polices de caractères
constexpr int FONT_SIZE_LARGE = 70; // Grand texte
constexpr int FONT_SIZE_MEDIUM = 40; // Texte moyen
constexpr int FONT_SIZE_SMALL = 28; // Petit texte
constexpr int FONT_SIZE_TINY = 20; // Très petit texte

// Taille des sprites
constexpr int PLAYER_PORTRAIT_SIZE = 60; // Taille du portrait du joueur
constexpr int ENEMY_SIZE = 60; // Taille initiale des ennemis
constexpr int BOSS_SIZE = 100; // Taille du boss

// Paramètres de spawn des ennemis
constexpr int ENEMY_SPAWN_BASE_ROWS = 3; // Nombre de rangées de base pour le spawn d'ennemis
constexpr int ENEMY_SPAWN_BASE_COLS = 5; // Nombre de colonnes de base pour le spawn d'ennemis
constexpr int ENEMY_SPAWN_MAX_ROWS = 6; // Nombre maximum de rangées pour le spawn d'ennemis
constexpr int ENEMY_SPAWN_MAX_COLS = 9; // Nombre maximum de colonnes pour le spawn d'ennemis
constexpr int ENEMY_SPACING_X = 20; // Espacement horizontal entre les ennemis
constexpr int ENEMY_SPACING_Y = 20; // Espacement vertical entre les ennemis

// Probabilités
constexpr float BONUS_DROP_CHANCE = 0.2f; // Chance de drop d'un bonus
constexpr float ENEMY_SHOOT_CHANCE_BASE = 0.1f; // Chance de tir basique des ennemis

// États du jeu
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

// Configuration du monde
struct WorldConfig {
    std::string name; // Nom du monde
    std::string background; // Fond d'écran du monde
    int levels; // Nombre de niveaux dans le monde
    std::string enemiesDir; // Répertoire des ennemis
    int enemyCount; // Nombre total d'ennemis dans le monde
    std::pair<Color, Color> playerBulletColors; // Couleurs des balles du joueur
    std::pair<Color, Color> enemyBulletColors; // Couleurs des balles des ennemis
};

// Données des mondes
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
