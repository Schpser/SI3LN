// Stubs.cpp

// Fichiers stubs pour une future implémentation
#include <SDL2/SDL.h>
#include <string>

namespace SI3LN {

// Stub de Renderer
class Renderer {
public:
    Renderer(SDL_Renderer* renderer) {} // Constructeur prenant un pointeur vers le renderer SDL
};

// Stub d'InputManager
class InputManager {
public:
    void update() {} // Méthode pour mettre à jour les entrées
};

// Stub de CollisionManager
class CollisionManager {
public:
    void checkCollisions() {} // Méthode pour vérifier les collisions
};

// Stub d'EntityManager
class EntityManager {
public:
    void update() {} // Méthode pour mettre à jour les entités
};

// Stub d'AssetManager
class AssetManager {
public:
    AssetManager(SDL_Renderer* renderer) {} // Constructeur prenant un pointeur vers le renderer SDL
};

// Stub d'AudioManager
class AudioManager {
public:
    void playSound(const std::string& name) {} // Méthode pour jouer un son en fonction de son nom
};

// Stub de ScoreManager
class ScoreManager {
public:
    void addScore(int score) {} // Méthode pour ajouter des points au score
};

// Stub d'AuthSystem
class AuthSystem {
public:
    bool login(const std::string& user, const std::string& pass) { return true; } // Méthode de connexion simulée
};

} // namespace SI3LN