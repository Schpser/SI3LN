#pragma once // Évite les inclusions multiples du fichier

#include "Entity.h" // Inclusion de la classe de base Entity
#include "Constants.h" // Inclusion des constantes et types utilisés
#include <SDL2/SDL_image.h> // Inclusion pour la gestion des textures SDL

namespace SI3LN {

// Classe représentant un ennemi dans le jeu
class Enemy : public Entity {
public:
    // Constructeur : initialise la position, la texture, la largeur d'écran et le niveau
    Enemy(float x, float y, SDL_Texture* texture, int screenWidth, int level);
    ~Enemy() override = default; // Destructeur par défaut
    
    // Met à jour la position et l'état de l'ennemi à chaque frame
    void update(float deltaTime) override;
    // Affiche l'ennemi à l'écran
    void render(SDL_Renderer* renderer) override;
    
    // Indique si l'ennemi peut tirer un projectile
    bool canShoot();
    
private:
    SDL_Texture* texture; // Texture de l'ennemi
    int screenWidth; // Largeur de l'écran pour gérer les déplacements
    float speed; // Vitesse de déplacement de l'ennemi
    int direction; // Direction du déplacement (1 = droite, -1 = gauche)
    int dropDistance; // Distance de descente lors du changement de direction
    int level; // Niveau actuel du jeu (influence la difficulté)
    
    uint32_t lastShootTime; // Temps du dernier tir
    uint32_t shootCooldown; // Délai entre deux tirs
    float shootChance; // Probabilité que l'ennemi tire
    
    int minX, maxX, maxY; // Limites de déplacement sur l'écran
};

} // namespace SI3LN // Fin de l'espace de noms SI3LN