#include "Bullet.h"

namespace SI3LN {

Bullet::Bullet(float x, float y, bool isPlayerBullet, int screenHeight, const Color& color)
    : Entity(x, y, 15, 25)
    , playerBullet(isPlayerBullet)
    , screenHeight(screenHeight)
    , color(color)
{
    if (isPlayerBullet) {
        speed = -PLAYER_BULLET_SPEED; // Negative = upward
        width = 15;
        height = 25;
    } else {
        speed = ENEMY_BULLET_SPEED; // Positive = downward
        width = 10;
        height = 20;
    }
}

void Bullet::update(float deltaTime) {
    position.y += speed * deltaTime * 60.0f; // Scale for frame independence
    
    // Remove if off screen
    if (position.y + height / 2 < 0 || position.y - height / 2 > screenHeight) {
        alive = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_Rect rect = getRect();
    
    // Draw outer ellipse
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Simple rectangle for now (could be enhanced with SDL_gfx for ellipses)
    SDL_RenderFillRect(renderer, &rect);
    
    // Draw inner highlight (lighter color)
    SDL_Rect innerRect = {
        rect.x + 2,
        rect.y + 3,
        rect.w - 4,
        rect.h - 6
    };
    
    uint8_t lighterR = std::min(255, color.r + 50);
    uint8_t lighterG = std::min(255, color.g + 50);
    uint8_t lighterB = std::min(255, color.b + 50);
    
    SDL_SetRenderDrawColor(renderer, lighterR, lighterG, lighterB, color.a);
    SDL_RenderFillRect(renderer, &innerRect);
}

} // namespace SI3LN
