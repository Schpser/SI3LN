#include "Enemy.h"
#include <algorithm>
#include <cstdlib>

namespace SI3LN {

Enemy::Enemy(float x, float y, SDL_Texture* texture, int screenWidth, int level)
    : Entity(x, y, ENEMY_SIZE, ENEMY_SIZE)
    , texture(texture)
    , screenWidth(screenWidth)
    , speed(ENEMY_SPEED + level * 0.2f)
    , direction(1)
    , dropDistance(20)
    , level(level)
    , lastShootTime(0)
    , shootCooldown(std::max(700, 3000 - level * 100))
    , shootChance(std::min(0.1f * level, 0.5f))
{
    // Set boundaries
    minX = 20;
    maxX = screenWidth - 20;
    maxY = screenWidth / 2;
}

void Enemy::update(float deltaTime) {
    // Horizontal movement
    position.x += speed * direction * deltaTime * 60.0f; // Scale by 60 for frame-independence
    
    // Check boundaries and change direction
    if (position.x + width / 2 >= maxX && direction > 0) {
        direction = -1;
        position.y += dropDistance;
    } else if (position.x - width / 2 <= minX && direction < 0) {
        direction = 1;
        position.y += dropDistance;
    }
    
    // Keep within vertical bounds
    if (position.y + height / 2 > maxY) {
        position.y = maxY - height / 2;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    if (!texture) return;
    
    SDL_Rect destRect = getRect();
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

bool Enemy::canShoot() {
    uint32_t currentTime = SDL_GetTicks();
    if (currentTime - lastShootTime > shootCooldown) {
        float randomValue = static_cast<float>(rand()) / RAND_MAX;
        if (randomValue < shootChance) {
            lastShootTime = currentTime;
            return true;
        }
    }
    return false;
}

} // namespace SI3LN
