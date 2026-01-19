#pragma once

#include "Entity.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>

namespace SI3LN {

class Enemy : public Entity {
public:
    Enemy(float x, float y, SDL_Texture* texture, int screenWidth, int level);
    ~Enemy() override = default;
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    
    bool canShoot();
    
private:
    SDL_Texture* texture;
    int screenWidth;
    float speed;
    int direction; // 1 for right, -1 for left
    int dropDistance;
    int level;
    
    // Shooting
    uint32_t lastShootTime;
    uint32_t shootCooldown;
    float shootChance;
    
    // Boundaries
    int minX, maxX, maxY;
};

} // namespace SI3LN
