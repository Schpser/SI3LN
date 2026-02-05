#pragma once // Évite les inclusions multiples du fichier

#include <string> // Pour la gestion des chaînes de caractères
#include <map> // Pour la gestion des mondes sous forme de dictionnaire
#include <SDL2/SDL.h> // Pour la gestion des couleurs et du rendu

namespace SI3LN {

// ==================== Paramètres d'écran ====================

constexpr int DEFAULT_SCREEN_WIDTH = 1920; //longeur par defaut de la fenetre de jeu
constexpr int DEFAULT_SCREEN_HEIGHT = 1080; // Hauteur par défaut de la fenêtre du jeu
constexpr int FPS = 60; // Nombre d'images par seconde
constexpr int FRAME_DELAY = 1000 / FPS; // Délai entre chaque image pour maintenir le FPS

// ==================== Structure de couleur ====================
// Structure représentant une couleur RGBA
struct Color {
    uint8_t r, g, b, a; // Composantes rouge, vert, bleu, alpha
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
    SDL_Color toSDL() const { return {r, g, b, a}; } // Conversion en SDL_Color
};

// ==================== Couleurs prédéfinies ====================
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

// ==================== Paramètres du jeu ====================
constexpr int MAX_LIVES = 5; // Nombre maximum de vies du joueur
constexpr int PLAYER_SPEED = 150; // Vitesse du joueur (réduite de 500 -> 150)
constexpr int ENEMY_SPEED = 1; // Vitesse initiale des ennemis
constexpr int PLAYER_BULLET_SPEED = 10; // Vitesse des projectiles du joueur
constexpr int ENEMY_BULLET_SPEED = 5; // Vitesse des projectiles des ennemis

// ==================== Durées et délais ====================
constexpr int SHIELD_DURATION = 3000; // Durée du bouclier (ms)
constexpr int MEGA_SHOT_DURATION = 5000; // Durée du méga tir (ms)
constexpr int SPECIAL_ATTACK_COOLDOWN = 10000; // Délai entre attaques spéciales (ms)

// ==================== Tailles des polices ====================
constexpr int FONT_SIZE_LARGE = 70; // Grande police
constexpr int FONT_SIZE_MEDIUM = 40; // Police moyenne
constexpr int FONT_SIZE_SMALL = 28; // Petite police
constexpr int FONT_SIZE_TINY = 20; // Très petite police

// ==================== Tailles des sprites ====================
constexpr int PLAYER_PORTRAIT_SIZE = 60; // Taille du portrait du joueur
constexpr int ENEMY_SIZE = 60; // Taille des ennemis
constexpr int BOSS_SIZE = 100; // Taille du boss

// ==================== Paramètres de spawn des ennemis ====================
constexpr int ENEMY_SPAWN_BASE_ROWS = 3; // Rangées de base pour le spawn
constexpr int ENEMY_SPAWN_BASE_COLS = 5; // Colonnes de base pour le spawn
constexpr int ENEMY_SPAWN_MAX_ROWS = 6; // Rangées max pour le spawn
constexpr int ENEMY_SPAWN_MAX_COLS = 9; // Colonnes max pour le spawn
constexpr int ENEMY_SPACING_X = 20; // Espacement horizontal entre ennemis
constexpr int ENEMY_SPACING_Y = 20; // Espacement vertical entre ennemis

// ==================== Probabilités ====================
constexpr float BONUS_DROP_CHANCE = 0.2f; // Probabilité d'obtenir un bonus
constexpr float ENEMY_SHOOT_CHANCE_BASE = 0.1f; // Probabilité de tir des ennemis

// ==================== États du jeu ====================
// Enumération des différents états possibles du jeu
enum class GameState {
    MAIN_MENU, // Menu principal
    LOGIN, // Connexion
    REGISTER, // Inscription
    PLAYER_SELECT, // Sélection du joueur
    LEVEL_SELECT, // Sélection du niveau
    GAMEPLAY, // Jeu en cours
    PAUSE, // Pause
    LEVEL_WIN, // Victoire du niveau
    GAME_OVER, // Fin du jeu
    PROFILE, // Profil du joueur
    HELP // Aide
};

// ==================== Configuration des mondes ====================
// Structure contenant la configuration d'un monde
struct WorldConfig {
    std::string name; // Nom du monde
    std::string background; // Image de fond du monde
    int levels; // Nombre de niveaux dans le monde
    std::string enemiesDir; // Dossier des ennemis
    int enemyCount; // Nombre total d'ennemis
    std::pair<Color, Color> playerBulletColors; // Couleurs des projectiles du joueur
    std::pair<Color, Color> enemyBulletColors; // Couleurs des projectiles des ennemis
};

// ==================== Données des mondes ====================
// Fonction qui retourne la configuration de tous les mondes disponibles
inline std::map<std::string, WorldConfig> getWorldsConfig() {
    std::map<std::string, WorldConfig> worlds;
    // Configuration du monde "Space"
    worlds["Space"] = {
        "Space World",
        "background_space.jpg",
        5,
        "Space_world",
        15,
        {Color(0, 150, 255), Color(100, 200, 255)},
        {Color(255, 50, 150), Color(255, 150, 200)}
    };
    // Configuration du monde "Desert"
    worlds["Desert"] = {
        "Desert World",
        "background_desert.png",
        5,
        "Desert_world",
        8,
        {Color(255, 200, 0), Color(255, 255, 100)},
        {Color(200, 100, 0), Color(255, 150, 50)}
    };
    // Configuration du monde "Forest"
    worlds["Forest"] = {
        "Forest World",
        "background_forest.png",
        5,
        "Forest_world",
        9,
        {Color(50, 255, 150), Color(150, 255, 200)},
        {Color(150, 50, 200), Color(200, 100, 255)}
    };
    // Configuration du monde "Marine"
    worlds["Marine"] = {
        "Marine World",
        "background_marine.jpg",
        5,
        "Marine_world",
        12,
        {Color(0, 255, 255), Color(150, 255, 255)},
        {Color(255, 100, 0), Color(255, 200, 100)}
    };
    // Configuration du monde "Apocalyptic"
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

} // namespace SI3LN // Fin de l'espace de noms SI3LN
