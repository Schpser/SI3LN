// Enemy.cpp

#include "Enemy.h"
#include <algorithm>
#include <cstdlib>

namespace SI3LN {

// Constructeur de la classe Enemy
Enemy::Enemy(float x, float y, SDL_Texture* texture, int screenWidth, int level)
    : Entity(x, y, ENEMY_SIZE, ENEMY_SIZE) // Initialisation de la classe Entity avec position et taille
    , texture(texture) // Stocke le pointeur vers l'image du monstre
    , screenWidth(screenWidth) // Stocke la largeur de l'écran
    , speed(ENEMY_SPEED + level * 0.2f) // Calcule la vitesse en fonction du niveau
    , direction(1) // Direction initiale (à droite)
    , dropDistance(20) // Distance à laquelle le monstre se déplace vers le bas après un changement de direction
    , level(level) // Niveau actuel
    , lastShootTime(0) // Dernière fois que le monstre a tiré
    , shootCooldown(std::max(700, 3000 - level * 100)) // Cooldown entre les tirs en fonction du niveau
    , shootChance(std::min(0.1f * level, 0.5f)) // Probabilité de tir en fonction du niveau
{
    // Définit les limites horizontales
    minX = 20;
    maxX = screenWidth - 20;
    maxY = screenWidth / 2; // Limite verticale
}

// Met à jour la position et le comportement du monstre
void Enemy::update(float deltaTime) {
    // Déplacement horizontal
    position.x += speed * direction * deltaTime * 60.0f; // Mise à l'échelle par 60 pour indépendance des cadres
    
    // Vérifie les limites horizontales et change de direction si nécessaire
    if (position.x + width / 2 >= maxX && direction > 0) {
        direction = -1; // Change de direction vers la gauche
        position.y += dropDistance; // Descend de la distance spécifiée
    } else if (position.x - width / 2 <= minX && direction < 0) {
        direction = 1; // Change de direction vers la droite
        position.y += dropDistance; // Descend de la distance spécifiée
    }
    
    // Garde le monstre à l'intérieur des limites verticales
    if (position.y + height / 2 > maxY) {
        position.y = maxY - height / 2; // Ajuste pour rester en dessous de la limite
    }
}

// Affiche le monstre à l'écran
void Enemy::render(SDL_Renderer* renderer) {
    if (!texture) return; // Ne fait rien si aucune texture n'est définie
    
    SDL_Rect destRect = getRect(); // Obtient le rectangle englobant du monstre
    SDL_RenderCopy(renderer, texture, nullptr, &destRect); // Affiche la texture du monstre
}

// Vérifie si le monstre peut tirer un missile
bool Enemy::canShoot() {
    uint32_t currentTime = SDL_GetTicks(); // Obtient le temps actuel en millisecondes
    if (currentTime - lastShootTime > shootCooldown) { // Vérifie si la période de refroidissement est écoulée
        float randomValue = static_cast<float>(rand()) / RAND_MAX; // Génère une valeur aléatoire entre 0 et 1
        if (randomValue < shootChance) { // Vérifie si cette valeur est inférieure à la probabilité de tir
            lastShootTime = currentTime; // Met à jour le temps du dernier tir
            return true;
        }
    }
    return false;
}

} // namespace SI3LN