// Bullet.cpp

#include "Bullet.h"

namespace SI3LN {

// Constructeur de la classe Bullet
Bullet::Bullet(float x, float y, bool isPlayerBullet, int screenHeight, const Color& color)
    : Entity(x, y, 15, 25) // Initialisation de la classe Entity avec position et taille
    , playerBullet(isPlayerBullet) // Indique si le missile est du joueur ou non
    , screenHeight(screenHeight) // Stocke la hauteur de l'écran pour vérification des limites
    , color(color) // Stocke la couleur du missile
{
    if (isPlayerBullet) {
        speed = -PLAYER_BULLET_SPEED; // Vitesse négative signifie déplacement vers le haut
        width = 15;
        height = 25;
    } else {
        speed = ENEMY_BULLET_SPEED; // Vitesse positive signifie déplacement vers le bas
        width = 10;
        height = 20;
    }
}

// Met à jour la position du missile et vérifie s'il est hors écran
void Bullet::update(float deltaTime) {
    position.y += speed * deltaTime * 60.0f; // Déplace le missile en fonction de sa vitesse et du temps écoulé
    
    // Vérifie si le missile est hors écran (soit au-dessus, soit en dessous de l'écran)
    if (position.y + height / 2 < 0 || position.y - height / 2 > screenHeight) {
        alive = false; // Marque le missile pour être supprimé
    }
}

// Affiche le missile à l'écran
void Bullet::render(SDL_Renderer* renderer) {
    SDL_Rect rect = getRect(); // Obtient le rectangle englobant du missile
    
    // Dessine la partie externe du missile (avec un simple rectangle pour l'instant)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    // Dessine le reflet intérieur (couleur plus claire) pour donner de la profondeur
    SDL_Rect innerRect = {
        rect.x + 2,
        rect.y + 3,
        rect.w - 4,
        rect.h - 6
    };
    
    uint8_t lighterR = std::min(255, color.r + 50); // Calcule la composante rouge plus claire
    uint8_t lighterG = std::min(255, color.g + 50); // Calcule la composante verte plus claire
    uint8_t lighterB = std::min(255, color.b + 50); // Calcule la composante bleue plus claire
    
    SDL_SetRenderDrawColor(renderer, lighterR, lighterG, lighterB, color.a);
    SDL_RenderFillRect(renderer, &innerRect);
}

} // namespace SI3LN