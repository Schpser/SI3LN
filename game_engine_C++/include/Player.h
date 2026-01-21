#pragma once

#include "Entity.h"
#include "Constants.h"
#include <SDL2/SDL_image.h>

namespace SI3LN {

class Player : public Entity {
public:
    Player(float x, float y, SDL_Texture* texture, int screenWidth, int screenHeight);
    ~Player() override = default;
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    
    void move(float dx, float dy);
    void handleInput(const uint8_t* keyState);
    
    bool canShoot() const;
    void resetShootCooldown();
    
private:
    SDL_Texture* texture;
    int screenWidth;
    int screenHeight;
    float speed;
    
    // Movement boundaries
    int minX, maxX, minY, maxY;
    
    // Shooting
    uint32_t lastShootTime;
    uint32_t shootCooldown;
};

} // namespace SI3LN