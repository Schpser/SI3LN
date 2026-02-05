// Player.h

#pragma once

#include "Entity.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>

namespace SI3LN {

// Classe représentant le joueur dans le jeu
class Player : public Entity {
        // Indique si le coup spécial est prêt
        bool specialReady = false;

    public:
        // Déclenche le tir spécial si prêt
        bool useSpecial() {
            if (specialCharge >= 5) { // Par exemple, 5 ennemis tués
                specialCharge = 0;
                specialReady = false;
                return true;
            }
            return false;
        }
    // Jauge de coup spécial (augmente quand le joueur tue des ennemis)
    int specialCharge = 0;
public:
    // Constructeur de la classe Player
    Player(float x, float y, SDL_Texture* texture, int screenWidth, int screenHeight);
    
    // Destructeur par défaut
    ~Player() override = default;
    
    // Méthode pour mettre à jour le joueur
    void update(float deltaTime) override;
    
    // Méthode pour afficher le joueur
    void render(SDL_Renderer* renderer) override;
    
    // Méthode pour déplacer le joueur
    void move(float dx, float dy);
    
    // Méthode pour gérer les entrées du joueur
    void handleInput(const uint8_t* keyState);
    
    // Vérifie si le joueur peut tirer un missile
    bool canShoot() const;
    
    // Réinitialise la période de refroidissement entre les tirs
    void resetShootCooldown();
    
private:
    SDL_Texture* texture; // Pointeur vers l'image du joueur
    int screenWidth; // Largeur de l'écran
    int screenHeight; // Hauteur de l'écran
    float speed; // Vitesse du joueur
    
    // Limites de mouvement horizontales et verticales
    int minX, maxX, minY, maxY;
    
    // Gestion des tirs
    uint32_t lastShootTime; // Dernière fois que le joueur a tiré
    uint32_t shootCooldown = 150; // Délai en ms entre deux tirs (150ms par défaut)
};

} // namespace SI3LN