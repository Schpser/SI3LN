#include "Player.h"

namespace SI3LN {

Player::Player(float x, float y, SDL_Texture* texture, int screenWidth, int screenHeight)
    : Entity(x, y, PLAYER_PORTRAIT_SIZE, PLAYER_PORTRAIT_SIZE)
    , texture(texture)
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , Speed(100.0f) // Vitesse initiale augmentée
    , lastShootTime(0)
    , shootCooldown(200) // 200ms between shots
{
    // Set movement boundaries
    minX = 0;
    maxX = screenWidth;
    minY = screenHeight / 2; // Can't go above middle
    maxY = screenHeight - 20;
}

void Player::update(float deltaTime) {
 // Update position based on velocity
    position += velocity * Speed * deltaTime; // Utilisation de deltaTime pour ajuster la vitesse

    // Clamp position to boundaries
    if (position.x - width / 2 < minX) position.x = minX + width / 2;
    if (position.x + width / 2 > maxX) position.x = maxX - width / 2;
    if (position.y - height / 2 < minY) position.y = minY + height / 2;
    if (position.y + height / 2 > maxY) position.y = maxY - height / 2;
}

void Player::render(SDL_Renderer* renderer) {
    if (!texture) return;
    
    SDL_Rect destRect = getRect();
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void Player::move(float dx, float dy) {    
    velocity.x = dx * Speed; 60.0f; // Utiliser une variable personnalisée pour la vitesse
    velocity.y = dy * Speed; 60.0f; // Utiliser une variable personnalisée pour la vitesse
}

void Player::handleInput(const uint8_t* keyState) {
    float dx = 100.0f;
    float dy = 100.0f;
    
    // Horizontal movement
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        dx = -100.0f;
    }
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        dx = 100.0f;
    }
    
    // Vertical movement
    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
        dy = -100.0f;
    }
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
        dy = 100.0f;
    }
    
    move(dx, dy);
}

bool Player::canShoot() const {
    uint32_t currentTime = SDL_GetTicks();
    return (currentTime - lastShootTime) >= shootCooldown;
}

void Player::resetShootCooldown() {
    lastShootTime = SDL_GetTicks();
}

} // namespace SI3LN
