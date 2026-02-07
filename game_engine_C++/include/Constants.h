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
constexpr int ENEMY_SPEED = 2; // Vitesse initiale des ennemis
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
        "background_marine.png",
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

// ==================== Descriptions des niveaux ====================
// Fonction qui retourne la description d'un niveau pour un monde donné
inline std::string getLevelDescription(const std::string& world, int level) {
    // Descriptions pour chaque monde et niveau
    std::map<std::string, std::map<int, std::string>> descriptions;
    
    // Space World
    descriptions["Space"][1] = "Bienvenue dans l'espace infini!\nVos premiers ennemis aliens approchent.\nUtilisez les fleches pour vous deplacer\net ESPACE pour tirer.";
    descriptions["Space"][2] = "Les aliens deviennent plus agressifs!\nIls tirent plus frequemment.\nRestez vigilant et esquivez leurs tirs.";
    descriptions["Space"][3] = "La flotte spatiale se renforce!\nPlus d'ennemis arrivent en formation.\nTenez bon, pilote!";
    descriptions["Space"][4] = "Alerte rouge! Invasion massive!\nLes aliens ont double leurs forces.\nVotre habilete sera mise a l'epreuve.";
    descriptions["Space"][5] = "Mission finale: Vaisseau-mere en vue!\nC'est le moment de prouver votre valeur.\nBonne chance, commandant!";
    
    // Desert World
    descriptions["Desert"][1] = "Bienvenue dans le desert brulant!\nLes creatures du sable emergent.\nLa chaleur est intense, restez concentre!";
    descriptions["Desert"][2] = "Tempete de sable en approche!\nLa visibilite diminue, mais\nles ennemis restent dangereux.";
    descriptions["Desert"][3] = "Les scorpions geants attaquent!\nIls sont rapides et mortels.\nNe les laissez pas s'approcher!";
    descriptions["Desert"][4] = "L'oasis cache des dangers!\nLes predateurs du desert sont feroces.\nProtegez votre vaisseau!";
    descriptions["Desert"][5] = "Le Sphinx se reveille!\nAffrontez le gardien du desert.\nSeuls les plus braves survivront!";
    
    // Forest World
    descriptions["Forest"][1] = "Entrez dans la foret enchantee!\nLes creatures magiques rodent.\nMefiez-vous des ombres...";
    descriptions["Forest"][2] = "Les feux follets vous guettent!\nIls sont rapides et imprevisibles.\nGardez vos reflexes affutes!";
    descriptions["Forest"][3] = "Les ents antiques s'eveillent!\nCes gardiens protegent la foret.\nIls ne vous laisseront pas passer.";
    descriptions["Forest"][4] = "Le coeur de la foret est menace!\nLes creatures sombres envahissent.\nDefendez ce lieu sacre!";
    descriptions["Forest"][5] = "L'Esprit de la Foret apparait!\nCette entite millénaire teste\nvotre courage. Soyez pret!";
    
    // Marine World
    descriptions["Marine"][1] = "Plongez dans les abysses!\nLes creatures marines emergent.\nL'ocean cache bien des secrets...";
    descriptions["Marine"][2] = "Les meduses phosphorescentes!\nLeurs tentacules sont mortels.\nEvitez tout contact!";
    descriptions["Marine"][3] = "Les requins mecaniques approchent!\nIls sont programmes pour detruire.\nNe leur tournez pas le dos!";
    descriptions["Marine"][4] = "La cite engloutie se defend!\nLes gardiens sous-marins attaquent.\nLa pression augmente!";
    descriptions["Marine"][5] = "Le Kraken emerge des profondeurs!\nCe leviathan est une legende.\nAffrontez votre destin!";
    
    // Apocalyptic World
    descriptions["Apocalyptic"][1] = "Bienvenue dans le monde ravage!\nLes mutants survivants attaquent.\nChaque ressource compte ici.";
    descriptions["Apocalyptic"][2] = "Les radiations augmentent!\nLes creatures sont plus fortes.\nAdaptez-vous ou perissez!";
    descriptions["Apocalyptic"][3] = "Les machines de guerre s'activent!\nL'ancienne technologie est mortelle.\nDetruisez-les avant qu'il soit trop tard!";
    descriptions["Apocalyptic"][4] = "Le bunker est en vue!\nMais il est bien defendu.\nForcez le passage!";
    descriptions["Apocalyptic"][5] = "L'IA supreme vous attend!\nElle a cause l'apocalypse.\nC'est l'heure de la vengeance!";
    
    // Retourner la description ou un texte par defaut
    if (descriptions.find(world) != descriptions.end() &&
        descriptions[world].find(level) != descriptions[world].end()) {
        return descriptions[world][level];
    }
    return "Niveau " + std::to_string(level) + "\nPreparez-vous au combat!\nBonne chance!";
}

} // namespace SI3LN // Fin de l'espace de noms SI3LN
