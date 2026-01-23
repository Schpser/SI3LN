#include "Player.h" // Inclusion de l'en-tête Player.h pour accéder aux déclarations précédentes

namespace SI3LN {

// Constructeur de la classe Player
Player::Player(float x, float y, SDL_Texture* texture, int screenWidth, int screenHeight)
    : Entity(x, y, PLAYER_PORTRAIT_SIZE, PLAYER_PORTRAIT_SIZE) // Initialisation des membres en utilisant le constructeur de la classe mère Entity
    , texture(texture) // Assignation du pointeur vers l'image du joueur
    , screenWidth(screenWidth) // Stockage de la largeur de l'écran
    , screenHeight(screenHeight) // Stockage de la hauteur de l'écran
    , speed(100.0f) // Vitesse initiale du joueur (non modifiée par le commentaire)
    , lastShootTime(0) // Dernier temps où un tir a été effectué
    , shootCooldown(200) // Cooldown entre les tirs en millisecondes
{
    // Définition des limites de déplacement du joueur
    minX = 0;
    maxX = screenWidth;
    minY = screenHeight / 2; // Ne peut pas aller au-dessus du milieu de l'écran
    maxY = screenHeight - 20; // Ne peut pas aller en dessous de 20 pixels du bas de l'écran
}

// Méthode pour mettre à jour la position du joueur en fonction du temps écoulé (deltaTime)
void Player::update(float deltaTime) {
    // Mettre à jour la position en utilisant la vitesse multipliée par le temps écoulé
    position += velocity * deltaTime; 
    
    // Clamper la position aux limites définies pour empêcher le joueur de sortir des bords de l'écran
    if (position.x - width / 2 < minX) position.x = minX + width / 2;
    if (position.x + width / 2 > maxX) position.x = maxX - width / 2;
    if (position.y - height / 2 < minY) position.y = minY + height / 2;
    if (position.y + height / 2 > maxY) position.y = maxY - height / 2;
}

// Méthode pour rendre le joueur sur l'écran
void Player::render(SDL_Renderer* renderer) {
    if (!texture) return; // Vérifie si le pointeur vers la texture est nul, sinon retourne sans rien faire
    
    SDL_Rect destRect = getRect(); // Obtenir les coordonnées rectangulaires du joueur
    SDL_RenderCopy(renderer, texture, nullptr, &destRect); // Copier la texture sur l'écran à ces coordonnées
}

// Méthode pour déplacer le joueur selon les directions données (dx et dy)
void Player::move(float dx, float dy) {    
    // Normaliser le mouvement diagonal pour conserver la même vitesse dans toutes les directions
    if (dx != 0.0f && dy != 0.0f) {
        dx *= 0.707f;
        dy *= 0.707f;
    }
    
    // Assigner la vitesse au vecteur de déplacement velocity
    velocity.x = dx * speed;
    velocity.y = dy * speed;
}

// Méthode pour gérer les entrées du joueur et mettre à jour sa direction de déplacement
void Player::handleInput(const uint8_t* keyState) {
    float dx = 0.0f;
    float dy = 0.0f;
    
    // Détecter les pressions de touches pour se déplacer vers la gauche ou la droite
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        dx = -1.0f;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        dx = 1.0f;
    }
    
    // Détecter les pressions de touches pour se déplacer vers le haut ou le bas
    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
        dy = -1.0f;
    }
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
        dy = 1.0f;
    }
    
    // Appeler la méthode move avec les valeurs de dx et dy calculées
    move(dx, dy);
}

// Méthode pour vérifier si le joueur peut tirer en fonction du cooldown actuel
bool Player::canShoot() const {
    uint32_t currentTime = SDL_GetTicks(); // Obtenir le temps actuel en millisecondes
    return (currentTime - lastShootTime) >= shootCooldown; // Vérifier si le temps écoulé depuis le dernier tir est supérieur ou égal au cooldown
}

// Méthode pour réinitialiser le cooldown après un tir
void Player::resetShootCooldown() {
    lastShootTime = SDL_GetTicks(); // Mettre à jour le dernier temps de tir avec le temps actuel
}

} // namespace SI3LN